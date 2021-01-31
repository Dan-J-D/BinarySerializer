/*
Source: https://github.com/Dan-J-D/BinarySerializer

Copyright 2021 Daniel Doych

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "BinarySerializer.h"

#include <stdlib.h>
#include <cstring>

BinarySerializer::BinarySerializer()
{
	this->serialized = (unsigned char*)0;
	this->serialized_size = 0;
}

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
