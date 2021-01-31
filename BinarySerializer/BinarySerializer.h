#pragma once

struct Serializable
{
	virtual unsigned char* Serialize(unsigned int* out_data_size) = 0;
	virtual bool Deserialize(unsigned char* data, unsigned int data_size) = 0;
};

class BinarySerializer
{
public:
	unsigned char* serialized;
	unsigned int serialized_size;

	BinarySerializer();
	~BinarySerializer();

	/* HINT: free after */
	void AddData(unsigned char* data, unsigned int data_size);
};

class BinaryDeserializer
{
public:
	unsigned char* serialized;
	unsigned int serialized_size;
	unsigned int read;

	BinaryDeserializer(unsigned char* serialized, unsigned int serialized_size);
	~BinaryDeserializer();

	unsigned char* ReadData(unsigned int* out_data_size);
};
