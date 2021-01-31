#include "BinarySerializer.h"

#include <stdlib.h>
#include <cstring>

BinarySerializer::BinarySerializer()
{
	this->serialized = (unsigned char*)0;
	this->serialized_size = 0;
}

/*BinarySerializer::BinarySerializer(unsigned int* serialized, unsigned int serialized_size)
{
	if (!this->serialized)
		free(this->serialized);
	this->serialized = serialized;
	this->serialized_size = serialized_size;
}*/

BinarySerializer::~BinarySerializer()
{
	if (this->serialized)
		free(this->serialized);
}

void BinarySerializer::AddData(unsigned char* data, unsigned int data_size)
{
	if (data_size <= 0) return;
	unsigned int index = 0;
	if (!this->serialized)
	{
		unsigned int size = data_size + sizeof(unsigned int);
		this->serialized = (unsigned char*)malloc(size);
		this->serialized_size = size;
	}
	else
	{
		unsigned char* old_serialized = this->serialized;
		unsigned int size = serialized_size + data_size + sizeof(unsigned int);
		this->serialized = (unsigned char*)malloc(size);
		memcpy(this->serialized, old_serialized, this->serialized_size);
		index = this->serialized_size;
		this->serialized_size = size;
	}
	memcpy(this->serialized + index, &data_size, sizeof(unsigned int));
	memcpy(this->serialized + index + sizeof(unsigned int), data, data_size);
}

BinaryDeserializer::BinaryDeserializer(unsigned char* serialized, unsigned int serialized_size)
{
	this->serialized = (unsigned char*)malloc(serialized_size);
	memcpy(this->serialized, serialized, serialized_size);
	this->serialized_size = serialized_size;
	this->read = 0;
}

BinaryDeserializer::~BinaryDeserializer()
{
	if (this->serialized)
		free(this->serialized);
}

unsigned char* BinaryDeserializer::ReadData(unsigned int* out_data_size)
{
	if (this->serialized_size - this->read < sizeof(unsigned int)) return 0;
	memcpy(out_data_size, serialized + read, sizeof(unsigned int));
	if (this->serialized_size - read < *out_data_size) return 0;
	unsigned char* data = (unsigned char*)malloc(*out_data_size);
	memcpy(data, this->serialized + read + sizeof(unsigned int), *out_data_size);
	read += sizeof(unsigned int) + *out_data_size;
	return data;
}
