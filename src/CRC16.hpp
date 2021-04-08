#pragma once
#include <stdint.h>

/// <summary>
/// FORWARD DECLARATIONS
/// </summary>

struct CRC16_LUT{
  uint16_t table[256];

  CRC16_LUT();

  uint16_t& operator[] (uint8_t index);
};

template <typename any_type>
uint16_t Slow_CRC16(any_type input_variable);

template <typename any_type>
uint16_t Fast_CRC16_fromvar(any_type input_variable);

uint16_t Fast_CRC16_fromarray(uint8_t* data_string_litteral, uint8_t byte_size);

void CalculateTable_CRC16(uint16_t crctable16[256]);


/// <summary>
/// DEFINITIONS
/// </summary>
/// <param name="crctable16"></param>
void CalculateTable_CRC16(uint16_t crctable16[256]) {
	const uint16_t generator = 0x1021;
	const uint16_t checker = 0x8000; // or 0x8000
	for (int divident = 0; divident < 256; divident++) { /* iterate over all possible input byte values 0 - 255 */
		uint16_t curByte = (uint16_t(divident << 8)); /* move divident byte into MSB of 16Bit CRC */
		for (uint8_t bit = 0; bit < 8; bit++) {
			if ((curByte & checker) != 0) {
				curByte <<= 1;
				curByte ^= generator;
			}
			else {
				curByte <<= 1;
			}
		}
		crctable16[divident] = curByte;
	}
}

/// <summary>
/// CRC16_LUT
/// </summary>

uint16_t& CRC16_LUT::operator[] (uint8_t index) {
	return table[index];
}

CRC16_LUT::CRC16_LUT() {
	CalculateTable_CRC16(table);
}

template <typename any_type>
uint16_t Slow_CRC16(any_type input_variable) {
	uint8_t* data_string_litteral = reinterpret_cast<uint8_t* const>(&input_variable);
	uint16_t crc_data = 0xFFFF;
	for (uint8_t j = sizeof(input_variable); j > 0; j--) {
		crc_data ^= (data_string_litteral[j - 1] << 8);
		for (uint8_t i = 0; i < 8; i++) {
			crc_data = crc_data & 0x8000 ? (crc_data << 1) ^ 0x1021 : crc_data << 1;
		}
	}
	return crc_data;
}

template <typename any_type>
uint16_t Fast_CRC16_fromvar(any_type input_variable) {
	static CRC16_LUT crctable16;
	uint8_t* data_string_litteral = reinterpret_cast<uint8_t* const>(&input_variable);
	uint16_t crc_data = 0xFFFF;
	for (uint8_t j = sizeof(input_variable); j > 0; j--) {
		uint8_t b = data_string_litteral[j - 1];      /* XOR-in next input byte into MSB of crc, that's our new intermediate divident */
		uint8_t pos = (uint8_t)((crc_data >> 8) ^ b); /* equal: ((crc ^ (b << 8)) >> 8) */
		crc_data = (uint16_t)((crc_data << 8) ^ (uint16_t)(crctable16[pos]));
	}
	return crc_data;
}

uint16_t Fast_CRC16_fromarray(uint8_t* data_string_litteral, uint8_t byte_size) {
	static CRC16_LUT crctable16;
	uint16_t crc_data = 0xFFFF;
	for (uint8_t j = 0; j < byte_size; j++) {
		uint8_t b = data_string_litteral[j];      /* XOR-in next input byte into MSB of crc, that's our new intermediate divident */
		uint8_t pos = (uint8_t)((crc_data >> 8) ^ b); /* equal: ((crc ^ (b << 8)) >> 8) */
		crc_data = (uint16_t)((crc_data << 8) ^ (uint16_t)(crctable16[pos]));
	}
	return crc_data;
}
