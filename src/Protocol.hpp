#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include "Configuration.h"
#include <ArduinoJson.h>
#include "CRC16.hpp"


char protocolcustom_valid_id[2] = "1";

char* TypeNameCorrespundancy(char char_type_holder);

char TypeIDCorrespundancy(uint8_t variable);

void Uint16_2_CharArray(uint16_t variable, char holder[2]);

struct TypeHolder {
	uint8_t id;
	char val_type;
	uint8_t byte_size;
	uint8_t values[8]{ 0 };
	char* identifier_string;
	uint8_t available = 0;
	uint8_t send_request = 0;

	TypeHolder();

	char* typeName();

	void calcCRC(char crc[2]);

	template <typename any_type>
	void setType(any_type buffer_variable);

	template <typename any_type>
	any_type getValue();

	template <typename any_type>
	void setValue(any_type input_variable);

	void clearValue();
};

template <uint8_t ARRAY_LEN>
class TypesBuffer {

public:
	uint8_t length;
	TypeHolder list[ARRAY_LEN];
	TypeHolder empty;

public:
	TypesBuffer();

	TypeHolder& operator[] (uint8_t index);

	template <typename any_type>
	void append(uint8_t _id, const char* _identifier_string);

	void clear();

	void remove(uint8_t index);
};

struct JsMessageHolder {
	char incomingLine[PROTOCOL_INPUT_LINE_SIZE]{ 0 };
	int8_t layer = 0;
	uint8_t active = 0;
	uint8_t indexer = 0;

	JsMessageHolder();

	void activate();

	void addBit(char input_bit);

	void clear();
};

struct HexMessageHolder {
	uint8_t id;
	uint8_t byte_size;
	uint8_t values[8]{ 0 };
	uint8_t crc[2]{ 0 };
	uint8_t count_bytes = 0;
	uint8_t active = 0;

	HexMessageHolder();

	void activate();

	//template <typename any_type>
	//any_type getValue();

	uint16_t getCRC();

	uint16_t calcCRC();

	void setID(uint8_t input_bit, TypeHolder* Holder);

	void addBit(uint8_t input_bit);

	void clearValue();

	void clearCRC();

	void clear();
};

template <uint8_t ARRAY_LEN>
class BJ_ProtocolHandler : public TypesBuffer<ARRAY_LEN> {

public:

	uint8_t jsonAvailable = 0;
	StaticJsonDocument<PROTOCOL_INPUT_LINE_SIZE> jsonInput;

protected:

	StaticJsonDocument<PROTOCOL_OUTPUT_LINE_SIZE> jsonOutput;
	StaticJsonDocument<25> jsonAknowledger_document;


	Stream* _COMport;
	Stream* _DEBUGport;
	char CaptureSequence = '_';

	JsMessageHolder JsM_Holder;
	HexMessageHolder HxM_Holder;

	unsigned long last_byte_memo = 0;
	uint8_t jsonOutput_token = 0;
	unsigned long json_last_send_memo = 0;
	uint8_t hexOutput_token = 0;
	uint8_t hexOutput_id = 0;
	unsigned long hex_last_send_memo = 0;

	bool json_send_request = 0;
	//TypesBuffer<ARRAY_LEN> PayloadsList;

public:

	BJ_ProtocolHandler();

	void begin(Stream& COMport, Stream& DEBUGport);

	void SendHex(uint8_t ID);

	void Send_JSON();

	void AddKeyValue_JSON(char* key, char* value);

	void AddKeyValue_JSON(const char* key, const char* value);

	void AddKeyValue_JSON(const char* key, char* value);

	void AddKeyValue_JSON(char* key, const char* value);

	void Aknowledge_Json(bool VALID, const char* action_taken);

	void Aknowledge_Json(bool VALID, char* action_taken = protocolcustom_valid_id);

	void CommsHandler();

	void _DirectSendInput();
	void _DirectSendOutput();

private:

	void SendHex_Handler();

	void SendHex_internal(uint8_t ID);

	void Internal_Send_JSON();

	void hex_transfer(TypeHolder* input_struct);

	void Free_Incoming_Sequence();

	void Aggregate_Json(uint8_t incomingByte);;

	void Aggregate_Hex(uint8_t incomingByte);

	void Aggregate_HexAknowledge(uint8_t incomingByte);

	void Start_Sequence(uint8_t incomingByte);

	void Json_TokenHandler();

	void Json_TokenRelease();

	void Hex_TokenHandler();

	void Hex_TokenRelease();

	void Aknowledge_Binary(bool VALID);

	bool JsonPreChecker(char* line, uint8_t len);

	bool Internal_JsonIntrepreter();

	bool Internal_JsonAknowledger();
};

///
///DECLARATION
/// 

char* TypeNameCorrespundancy(char char_type_holder) {
	char* output_variable;
	switch (char_type_holder) {
	case 'E':
		output_variable = const_cast<char*>("uint_8");
		break;
	case 'G':
		output_variable = const_cast<char*>("uint_16");
		break;
	case 'I':
		output_variable = const_cast<char*>("uint_32");
		break;
	case 'K':
		output_variable = const_cast<char*>("uint_64");
		break;
	case 'C':
		output_variable = const_cast<char*>("int_8");
		break;
	case 'F':
		output_variable = const_cast<char*>("int_16");
		break;
	case 'H':
		output_variable = const_cast<char*>("int_32");
		break;
	case 'J':
		output_variable = const_cast<char*>("int_64");
		break;
	case 'M':
		output_variable = const_cast<char*>("float_single");
		break;
	case 'N':
		output_variable = const_cast<char*>("float_double");
		break;
	default:
		output_variable = strcat(const_cast<char*>("unknown_type:"), reinterpret_cast<char*>(char_type_holder));
		break;
	}
	return output_variable;
}

char TypeIDCorrespundancy(uint8_t variable) {
	return 'E';
}
char TypeIDCorrespundancy(uint16_t variable) {
	return 'G';
}
char TypeIDCorrespundancy(uint32_t variable) {
	return 'I';
}
char TypeIDCorrespundancy(uint64_t variable) {
	return 'K';
}
char TypeIDCorrespundancy(int8_t variable) {
	return 'C';
}
char TypeIDCorrespundancy(int16_t variable) {
	return 'F';
}
char TypeIDCorrespundancy(int32_t variable) {
	return 'H';
}
char TypeIDCorrespundancy(int64_t variable) {
	return 'J';
}
char TypeIDCorrespundancy(float variable) {
	return 'M';
}
char TypeIDCorrespundancy(double variable) {
	return 'N';
}

void Uint16_2_CharArray(uint16_t variable, char holder[2]) {
	holder[1] = (char)(variable & 0x00FF);
	holder[0] = (char)((variable & 0xFF00) >> 8);
}

/// <summary>
/// TypeHolder
/// </summary>

TypeHolder::TypeHolder() {
	id = 0;
	val_type = '_';
	byte_size = 0;
	setValue<uint64_t>(0);
}

char* TypeHolder::typeName() {
	return TypeNameCorrespundancy(val_type);
}

void TypeHolder::calcCRC(char crc[2]) {
	uint16_t crc_data = Fast_CRC16_fromarray(values, byte_size);
	Uint16_2_CharArray(crc_data, crc);
}

template <typename any_type>
void TypeHolder::setType(any_type buffer_variable) {
	val_type = TypeIDCorrespundancy(buffer_variable);
	byte_size = sizeof(buffer_variable);
}

template <typename any_type>
any_type TypeHolder::getValue() {
	if (available){
		available = 0;
	}
	uint64_t buffer_variable = 0;
	for (uint8_t i = 0; i < sizeof(buffer_variable); i++) {
		buffer_variable = buffer_variable | (((uint64_t)values[i]) << 8 * i);
	}
	any_type output_variable = 0;
	memcpy(&output_variable, &buffer_variable, sizeof(output_variable));
	return output_variable;
}

template <typename any_type>
void TypeHolder::setValue(any_type input_variable) {
	uint64_t buffer_variable = 0;
	memcpy(&buffer_variable, &input_variable, sizeof(input_variable));
	for (uint8_t i = 0; i < sizeof(input_variable); i++) {
		values[i] = (uint8_t)(buffer_variable >> (8 * i));
	}
}


void TypeHolder::clearValue() {
	for (uint8_t i = 0; i < 8; i++) {
		values[i] = 0x00;
	}
}

/// <summary>
/// Types Buffer
/// </summary>

template <uint8_t ARRAY_LEN>
TypesBuffer<ARRAY_LEN>::TypesBuffer() {
	length = ARRAY_LEN;
	empty.val_type = '*';
	clear();
}

template <uint8_t ARRAY_LEN>
TypeHolder& TypesBuffer<ARRAY_LEN>::operator[] (uint8_t index)
{
	for (uint8_t i = 0; i < length; i++) {
		if (list[i].id == index) {
			return list[i];
		}
	}
	return empty;//RETURN 0 with typeholder empty with val_type = "*"
}

template <uint8_t ARRAY_LEN> template <typename any_type>
void TypesBuffer<ARRAY_LEN>::append(uint8_t _id, const char* _identifier_string) {
	for (uint8_t i = 0; i < length; i++) {
		if (list[i].val_type == '_') {
			list[i].id = _id;
			any_type buffer_variable = 0;
			list[i].setType(buffer_variable);
			list[i].identifier_string = const_cast<char*>(_identifier_string);
			return;
		}
	}
}

template <uint8_t ARRAY_LEN>
void TypesBuffer<ARRAY_LEN>::clear() {
	for (uint8_t i = 0; i < length; i++) {
		list[i].val_type = '_';
	}
}

template <uint8_t ARRAY_LEN>
void TypesBuffer<ARRAY_LEN>::remove(uint8_t index) {
	list[index].val_type = '_';
}

/// <summary>
/// JsMessageHolder
/// </summary>

JsMessageHolder::JsMessageHolder() {
	for (uint8_t i = 0; i < PROTOCOL_INPUT_LINE_SIZE; i++) {
		incomingLine[i] = 0xFF;
	}
}

void JsMessageHolder::activate() {
	active = 1;
}

void JsMessageHolder::addBit(char input_bit) {
	if (active == 1) {
		if (input_bit == 0x7B) {//{
			layer = layer + 1;
		}
		else if (input_bit == 0x7D) {//}
			layer = layer - 1;
		}
		incomingLine[indexer] = input_bit;
		indexer = indexer + 1;

		if (layer == 0) {
			active = 2;
		}
		if (layer < 0) {
			clear();
			active = 3;
		}
		if (indexer >= PROTOCOL_INPUT_LINE_SIZE) {
			indexer = PROTOCOL_INPUT_LINE_SIZE - 1;
			clear();
			active = 3;
		}
	}
}

void JsMessageHolder::clear() {
	for (uint8_t i = 0; i <= indexer; i++) {
		incomingLine[i] = 0xFF;
	}
	layer = 0;
	active = 0;
	indexer = 0;
}

/// <summary>
/// HexMessageHolder
/// </summary>

HexMessageHolder::HexMessageHolder() {
	id = 0;
	byte_size = 0;
	clearValue();
	clearCRC();
}

void HexMessageHolder::activate() {
	active = 1;
}

//template <typename any_type>
//any_type HexMessageHolder::getValue() {
//	uint64_t buffer_variable = 0;
//	for (uint8_t i = 0; i < sizeof(buffer_variable); i++) {
//		buffer_variable = buffer_variable | (((uint64_t)values[i]) << 8 * i);
//	}
//	any_type output_variable = 0;
//	memcpy(&output_variable, &buffer_variable, sizeof(output_variable));
//	return output_variable;
//}

uint16_t HexMessageHolder::getCRC() {
	uint16_t buffer_variable = 0;
	for (int8_t i = 0; i < 2; i++) {
		buffer_variable = buffer_variable | (((uint16_t)crc[i]) << (8 - (8 * i)));
	}
	return buffer_variable;
}

uint16_t HexMessageHolder::calcCRC() {
	return Fast_CRC16_fromarray(values, byte_size);
}

void HexMessageHolder::setID(uint8_t input_bit, TypeHolder* Holder) {
	if (Holder->val_type == '*' || Holder->val_type == '_') {// no correspunding id in bufferlist or TypeHolder with valid id uninitialized
		clear();
		active = 3;
	}
	else {
		id = input_bit;
		byte_size = Holder->byte_size;
		count_bytes = count_bytes + 1;
	}
}

void HexMessageHolder::addBit(uint8_t input_bit) {
	if (active == 1) {
		if (count_bytes > 0 && count_bytes <= byte_size) {
			values[count_bytes - 1] = input_bit;
		}
		else if (count_bytes <= byte_size + 2) {
			crc[count_bytes - (1 + byte_size)] = input_bit;
		}
		else {
			clear();
			active = 3;
		}

		if (count_bytes == byte_size + 2) {
			active = 2;
		}
		count_bytes = count_bytes + 1;
	}
}

void HexMessageHolder::clearValue() {
	for (uint8_t i = 0; i < 8; i++) {
		values[i] = 0x00;
	}
};

void HexMessageHolder::clearCRC() {
	for (uint8_t i = 0; i < 2; i++) {
		crc[i] = 0x00;
	}
};

void HexMessageHolder::clear() {
	clearValue();
	clearCRC();
	id = 0;
	byte_size = 0;
	count_bytes = 0;
	active = 0;
}


/// <summary>
/// BJ_ProtocolHandler
/// </summary>


template <uint8_t ARRAY_LEN>
BJ_ProtocolHandler<ARRAY_LEN>::BJ_ProtocolHandler() : TypesBuffer <ARRAY_LEN>() {}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::begin(Stream& COMport, Stream& DEBUGport) {
	_COMport = &COMport;
	_DEBUGport = &DEBUGport;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::hex_transfer(TypeHolder* input_struct) {
	for (uint8_t i = 0; i < input_struct->byte_size; i++) {
		input_struct->values[i] = HxM_Holder.values[i];
	}
	input_struct->available = 1;
	HxM_Holder.clear();
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Free_Incoming_Sequence() {
	if (PROTOCOL_DEBUG_MODE) {
		_DEBUGport->println("incoming_seq_freed");
	}
	CaptureSequence = '_';
	JsM_Holder.clear();
	HxM_Holder.clear();

}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Aggregate_Json(uint8_t incomingByte) {
	JsM_Holder.addBit((char)incomingByte);
	if (JsM_Holder.active == 2) {
		if (JsonPreChecker(JsM_Holder.incomingLine, JsM_Holder.indexer) == 1) {
			if(Internal_JsonIntrepreter()) {
				if (Internal_JsonAknowledger()) {
					jsonAvailable = 1;//flag to call JsonInterpreter() once in main loop;
					if (PROTOCOL_DEBUG_MODE) {
						_DEBUGport->print("Null ? : "); _DEBUGport->println(jsonInput.isNull()); 
						_DEBUGport->print(" with "); serializeJson(jsonInput, *_DEBUGport); _DEBUGport->println();
						_DEBUGport->print(" key ? "); _DEBUGport->println(jsonInput.containsKey("go"));
					}
				}
			}
		}
		Free_Incoming_Sequence();
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->print(" with "); serializeJson(jsonInput, *_DEBUGport); _DEBUGport->println();
		}
	}
	else if (JsM_Holder.active == 3) {
		Aknowledge_Json(false, "i");//integrity error
		Free_Incoming_Sequence();
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Send_JSON() {
	if (json_send_request == 0){
		json_send_request = 1;
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Internal_Send_JSON() {
	serializeJson(jsonOutput, *_COMport);
	jsonOutput_token = jsonOutput_token + 1;
	json_last_send_memo = micros();
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::AddKeyValue_JSON(char* key, char* value) {
	jsonOutput[key] = value;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::AddKeyValue_JSON(const char* key, const char* value) {
	jsonOutput[key] = value;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::AddKeyValue_JSON(char* key, const char* value) {
	jsonOutput[key] = value;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::AddKeyValue_JSON(const char* key, char* value) {
	jsonOutput[key] = value;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Aggregate_Hex(uint8_t incomingByte) {
	if (HxM_Holder.count_bytes == 0) {
		HxM_Holder.setID(incomingByte, &(this->operator[](incomingByte)));
	}
	else {
		HxM_Holder.addBit(incomingByte);
	}
	if (HxM_Holder.active == 2) {
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->print("CRC_got: "); _DEBUGport->print(HxM_Holder.getCRC(), HEX);
			_DEBUGport->print(" CRC_cal: ");_DEBUGport->println(HxM_Holder.calcCRC(), HEX);
		}
		if (HxM_Holder.calcCRC() == HxM_Holder.getCRC()) {

			hex_transfer(&(this->operator[](HxM_Holder.id)));
			Aknowledge_Binary(true);
			Free_Incoming_Sequence();
		}
		else {
			Aknowledge_Binary(false);
			Free_Incoming_Sequence();
		}
	}
	else if (HxM_Holder.active == 3) {
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("Binary data corrupted. NAK");
		}
		Aknowledge_Binary(false);
		Free_Incoming_Sequence();
	}
};

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Aggregate_HexAknowledge(uint8_t incomingByte) {
	if (incomingByte == 0x41) {//A
		Hex_TokenRelease();
	}
	else if (incomingByte == 0x6E) {//n
		Hex_TokenHandler();
	}
	Free_Incoming_Sequence();
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Start_Sequence(uint8_t incomingByte) {
	if (incomingByte == 0x7B) { // "{" Starts capturing Json Line
		CaptureSequence = 'j';
		JsM_Holder.activate();
		JsM_Holder.addBit((char)incomingByte);
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("incoming_seq_start_json");
		}
	}
	else if (incomingByte == PROTOCOL_BINARY_LINE_START) { // "#" Starts capturing Bytes (4 bytes + 2 CRC bytes (CRC16_b))
		CaptureSequence = 'b';
		HxM_Holder.activate();
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("incoming_seq_start_hex");
		}
	}
	else if (incomingByte == PROTOCOL_BINARY_AKNOWLEDGE_START) { // "@" Starts capturing 1byte for aknowledge
		CaptureSequence = 'a';
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("incoming_seq_start_aknow");
		}
	}
	else {
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("incoming_seq_start_error");
		}
		CaptureSequence = '_';
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Json_TokenHandler() {
	if (jsonOutput_token > 2) {
		Json_TokenRelease();
	}
	else if (jsonOutput_token > 0) {
		Internal_Send_JSON();
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Json_TokenRelease() {
	jsonOutput_token = 0;
	json_send_request = 0;
	jsonOutput.clear();
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Hex_TokenHandler() {
	if (hexOutput_token > 2) {
		Hex_TokenRelease();
	}
	else if (hexOutput_token > 0) {
		SendHex_internal(hexOutput_id);
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Hex_TokenRelease() {
	hexOutput_token = 0;
	hexOutput_id = 0;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Aknowledge_Binary(bool VALID) {
	_COMport->write(PROTOCOL_BINARY_AKNOWLEDGE_START);//0x7C
	if (VALID) {
		_COMport->write(PROTOCOL_BINARY_AKNOWLEDGE_BYTE); //A 0x41
	}
	else {
		_COMport->write(PROTOCOL_BINARY_NOT_AKNOWLEDGE_BYTE); //n 0x6E
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Aknowledge_Json(bool VALID, char* action_taken) {
	if (VALID) {
		jsonAknowledger_document[const_cast<char*>(PROTOCOL_JSON_AKNOWLEDGE_STR)] = action_taken;
	}
	else {
		jsonAknowledger_document[const_cast<char*>(PROTOCOL_JSON_NOT_AKNOWLEDGE_STR)] = action_taken;
	}
	serializeJson(jsonAknowledger_document, *_COMport);
	jsonAknowledger_document.clear();
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::Aknowledge_Json(bool VALID, const char* action_taken) {
	if (VALID) {
		jsonAknowledger_document[const_cast<char*>(PROTOCOL_JSON_AKNOWLEDGE_STR)] = action_taken;
	}
	else {
		jsonAknowledger_document[const_cast<char*>(PROTOCOL_JSON_NOT_AKNOWLEDGE_STR)] = action_taken;
	}
	serializeJson(jsonAknowledger_document, *_COMport);
	jsonAknowledger_document.clear();
}


template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::SendHex(uint8_t ID) {
	(this->operator[](ID)).send_request = 1;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::SendHex_internal(uint8_t ID) {
	char crc[2]{ 0 };
	(this->operator[](ID)).calcCRC(crc);
	_COMport->write(PROTOCOL_BINARY_LINE_START);
	_COMport->write(ID);
	_COMport->write((this->operator[](ID)).values, (this->operator[](ID)).byte_size);
	_COMport->write(crc, 2);
	(this->operator[](ID)).send_request = 0;
	hexOutput_id = ID;
	hex_last_send_memo = micros();
	hexOutput_token = hexOutput_token + 1;
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::SendHex_Handler() {
	if (hexOutput_token == 0) {
		for (uint8_t i = 0; i < this->length; i++) {
			if (this->list[i].send_request == 1) {
				SendHex_internal(this->list[i].id);
			}
		}
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::CommsHandler() {
	if (_COMport->available() > 0) {
		uint8_t incomingByte = _COMport->read();
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->print("DEC: "); _DEBUGport->print(incomingByte, DEC);_DEBUGport->print(" HEX: 0x");
			_DEBUGport->println(incomingByte, HEX);
		}
		switch (CaptureSequence) {
		case 'j':
			Aggregate_Json(incomingByte);
			break;
		case 'b':
			Aggregate_Hex(incomingByte);
			break;
		case 'a':
			Aggregate_HexAknowledge(incomingByte);
			break;
		default:
			Start_Sequence(incomingByte);
			break;
		}
		last_byte_memo = micros();
	}
	else if (CaptureSequence != '_' && micros() - last_byte_memo > PROTOCOL_TIMEOUT_MAX_BYTE_INTERVAL) {
		if (CaptureSequence == 'b') {
			Aknowledge_Binary(false);
		}
		else if (CaptureSequence == 'j') {
			Aknowledge_Json(false,"t");
		}
		Free_Incoming_Sequence();
	}

	if (hexOutput_token > 0 && micros() - hex_last_send_memo > PROTOCOL_TIMEOUT_BINARY_AK) {
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("bin_ak_timeout");
		}
		Hex_TokenHandler();
	}

	if (jsonOutput_token > 0 && micros() - json_last_send_memo > PROTOCOL_TIMEOUT_JSON_AK) {
		if (PROTOCOL_DEBUG_MODE) {
			_DEBUGport->println("json_ak_timeout");
		}
		Json_TokenHandler();
	}

	if (json_send_request && jsonOutput_token == 0) {

		Internal_Send_JSON();
	}

	SendHex_Handler();	

}


template <uint8_t ARRAY_LEN>
bool BJ_ProtocolHandler<ARRAY_LEN>::Internal_JsonAknowledger(){
	bool Ak_recieved = 0;
	if (jsonInput.containsKey(PROTOCOL_JSON_AKNOWLEDGE_STR)) {
		Json_TokenRelease();
		jsonInput.remove(PROTOCOL_JSON_AKNOWLEDGE_STR);
		Ak_recieved = 1;
	}
	else if (jsonInput.containsKey(PROTOCOL_JSON_NOT_AKNOWLEDGE_STR)) {
		Json_TokenHandler();
		jsonInput.remove(PROTOCOL_JSON_NOT_AKNOWLEDGE_STR);
		Ak_recieved = 1;
	}
	if (Ak_recieved) {
		if (! jsonInput.isNull()) {
			Ak_recieved = 0;
		}
	}
	return !Ak_recieved;
}

template <uint8_t ARRAY_LEN>
bool BJ_ProtocolHandler<ARRAY_LEN>::Internal_JsonIntrepreter() {
	bool ValidKey = 0;

	if (jsonInput.containsKey("get_vartype")) {
		if (jsonOutput_token == 0) {
			if (PROTOCOL_DEBUG_MODE) {
				_DEBUGport->print("vartype_called : ");
			}
			uint8_t varID = (uint8_t)jsonInput["get_vartype"];
			if (PROTOCOL_DEBUG_MODE) {
				_DEBUGport->print(varID);_DEBUGport->print(" ");
			}
			char* varname = (this->operator[](varID)).typeName();
			if (PROTOCOL_DEBUG_MODE) {
				_DEBUGport->println(varname);
			}
			/*char* strID = 0;
			strID = itoa(varID, strID, 10);
			strID = strcat(strID, "#");
			strID = strcat(strID, varname);*/
			AddKeyValue_JSON("var_type", varname);
			Send_JSON();
			ValidKey = 1;
		}
	}
	if (jsonInput.containsKey("get_varname")) {
		if (jsonOutput_token == 0) {
			uint8_t varID = (uint8_t)jsonInput["get_varname"];
			char* varname = (this->operator[](varID)).identifier_string;
			/*char* strID = 0;
			strID = itoa(varID, strID, 10);
			strID = strcat(strID, "#");
			strID = strcat(strID, varname);*/
			AddKeyValue_JSON("var_name", varname);
			Send_JSON();
			ValidKey = 1;
		}
	}
	if (jsonInput.containsKey("get_varvalue")) {
		if (jsonOutput_token == 0) {
			uint8_t varID = (uint8_t)jsonInput["get_varvalue"];
			/*char* varname = (this->operator[](varID)).identifier_string;
			char* strID = itoa(varID, strID, 10);
			varname = strcat(varname, "#");
			varname = strcat(varname, strID);*/
			char result[5];
			dtostrf(varID, 3, 0, result);
			AddKeyValue_JSON("var_value", result);
			Send_JSON();
			SendHex(varID);
			ValidKey = 1;
		}
	}
	if (ValidKey) {
		Aknowledge_Json(true);

	}
	return !ValidKey;
}

template <uint8_t ARRAY_LEN>
bool BJ_ProtocolHandler<ARRAY_LEN>::JsonPreChecker(char* line, uint8_t len) {
	if (len >= PROTOCOL_INPUT_LINE_SIZE) {
		Aknowledge_Json(false, "l");
		return 0;
	}
	else {
		const char* temp_const_line = line;// HACK : here, we convert from char* to const char* because otherwise
		// deserializeJson do zero-copy operation with char* (and not with const char*). This would lead to data supression when calling JsM_Holder.clear()
		// because pointer to line == pointer to JsM_Holder.incomingLine == pointer that produces the keys of jsonInput. Therefore in main loop we cannot acess jsonInput data anymore.
		auto error = deserializeJson(jsonInput, temp_const_line);
		if (error) {
			Aknowledge_Json(false, "d");// deserialisation error : not aknowledge
			return 0;
		}
		else {
			return 1;
		}
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::_DirectSendInput() {
	if (PROTOCOL_DEBUG_MODE){
		_DEBUGport->print("Current Input :"); serializeJson(jsonInput, *_DEBUGport); _DEBUGport->println();
	}
}

template <uint8_t ARRAY_LEN>
void BJ_ProtocolHandler<ARRAY_LEN>::_DirectSendOutput() {
	if (PROTOCOL_DEBUG_MODE) {
		_DEBUGport->print("Current Output :"); serializeJson(jsonOutput, *_DEBUGport); _DEBUGport->println();
	}
}




////
//// EXPLICIT INSTANTIATION OF TEMPLATES
////

template class BJ_ProtocolHandler<PROTOCOL_ITEM_NUMBER>;

template class TypesBuffer<PROTOCOL_ITEM_NUMBER>;

template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<uint8_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<uint16_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<uint32_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<uint64_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<int8_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<int16_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<int32_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<int64_t>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<float>(uint8_t _id, const char* _identifier_string);
template void TypesBuffer<PROTOCOL_ITEM_NUMBER>::append<double>(uint8_t _id, const char* _identifier_string);

template void TypeHolder::setType(uint8_t buffer_variable);
template void TypeHolder::setType(uint16_t buffer_variable);
template void TypeHolder::setType(uint32_t buffer_variable);
template void TypeHolder::setType(uint64_t buffer_variable);
template void TypeHolder::setType(int8_t buffer_variable);
template void TypeHolder::setType(int16_t buffer_variable);
template void TypeHolder::setType(int32_t buffer_variable);
template void TypeHolder::setType(int64_t buffer_variable);
template void TypeHolder::setType(float buffer_variable);
template void TypeHolder::setType(double buffer_variable);

template uint8_t TypeHolder::getValue();
template uint16_t TypeHolder::getValue();
template uint32_t TypeHolder::getValue();
template uint64_t TypeHolder::getValue();
template int8_t TypeHolder::getValue();
template int16_t TypeHolder::getValue();
template int32_t TypeHolder::getValue();
template int64_t TypeHolder::getValue();
template float TypeHolder::getValue();
template double TypeHolder::getValue();

template void TypeHolder::setValue(uint8_t input_variable);
template void TypeHolder::setValue(uint16_t input_variable);
template void TypeHolder::setValue(uint32_t input_variable);
template void TypeHolder::setValue(uint64_t input_variable);
template void TypeHolder::setValue(int8_t input_variable);
template void TypeHolder::setValue(int16_t input_variable);
template void TypeHolder::setValue(int32_t input_variable);
template void TypeHolder::setValue(int64_t input_variable);
template void TypeHolder::setValue(float input_variable);
template void TypeHolder::setValue(double input_variable);
