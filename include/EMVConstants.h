#ifndef __H_EMVCONSTANTS
#define __H_EMVCONSTANTS

enum INSValues {

   INS_APPLICATION_BLOCK = 0x1E,
   INS_APPLICATION_UNBLOCK = 0x18,
   INS_CARD_BLOCK = 0x16,
   INS_EXTERNAL_AUTHENTICATE = 0x82,
   INS_GENERATE_AC = 0xAE,
   INS_GET_CHALLENGE = 0x84,
   INS_GET_DATA = 0xCA,
   INS_GET_PROCESSING_OPTIONS = 0xA8,
   INS_INTERNAL_AUTHENTICATE = 0x88,
   INS_READ_RECORD = 0xB2,
   INS_PIN_CHANGE_UNBLOCK = 0x24,
   INS_SELECT = 0xA4,
   INS_VERIFY = 0x20

};

enum SW1Values {
   SW1_SUCCES = 0x90,
   SW1_WARNING_1 = 0x62,
   SW1_WARNING_2 = 0x63,
   SW1_EXECUTION_1 = 0x64,
   SW1_EXECUTION_2 = 0x65,
   SW1_CHECKING = 0x67
};

// Application info for select application - Credits to https://github.com/jbteam/libemv/blob/master/include/libemv.h
typedef struct
{
	int DFNameLength;
	unsigned char DFName[16];				// byte data with size DFNameLength
	char strApplicationLabel[17];			// Null terminated c string
	char needCardholderConfirm;				// 1 - need confirm, 0 - selection without confirmation
	int priority;							// 0 - no priority, else ranging from 1–15, with 1 being highest priority
	char strLanguagePreference[9];			// Null terminated c string, 1-4 languages stored in order of preference, each represented by 2 alphabetical characters according to ISO 639
	unsigned char issuerCodeTableIndex;		// Indicates the code table according to ISO/IEC 8859 for displaying the Application Preferred Name
	char strApplicationPreferredName[17];	// Preferred mnemonic associated with the AID, codepage: issuerCodeTableIndex
	int indexRID;							// For internal use
} ApplicationInfo;

// Tags - Credits to https://github.com/jbteam/libemv/blob/master/internal.h
#define TAG_FCI_TEMPLATE					0x6F
#define TAG_DF_NAME							0x84
#define TAG_FCI_PROP_TEMPLATE				0xA5
#define TAG_SFI_OF_DEF						0x88
#define TAG_LANGUAGE_PREFERENCE				0x5F2D
#define TAG_ISSUER_CODE_TABLE_INDEX			0x9F11
#define TAG_FCI_ISSUER_DISCR_DATA			0xBF0C
#define TAG_APPLICATION_LABEL				0x50
#define TAG_APP_PRIORITY_INDICATOR			0x87
#define TAG_PDOL							0x9F38
#define TAG_TVR								0x95
#define TAG_TSI								0x9B
#define TAG_APPLICATION_TEMPLATE			0x61
#define TAG_ADF_NAME						0x4F
#define TAG_APP_PREFERRED_NAME				0x9F12
#define TAG_TERMINAL_CAPABILITIES			0x9F33
#define TAG_ADDI_TERMINAL_CAPABILITIES		0x9F40
#define TAG_AID								0x9F06
#define TAG_IFD_SERIAL_NUMBER				0x9F1E
#define TAG_TERMINAL_COUNTRY_CODE			0x9F1A
#define TAG_TERMINAL_TYPE					0x9F35
#define TAG_ACQUIRER_ID						0x9F01
#define TAG_APPLICATION_VERSION_NUMBER		0x9F08
#define TAG_MCC								0x9F15
#define TAG_MERCHANT_ID						0x9F16
#define TAG_MERCHANT_NAME_AND_LOCATION		0x9F4E
#define TAG_TERMINAL_FLOOR_LIMIT			0x9F1B
#define TAG_TERMINAL_ID						0x9F1C
#define TAG_RISK_MANAGEMENT_DATA			0x9F1D
#define TAG_TRANSACTION_REFERENCE_CURRENCY	0x9F3C
#define TAG_TRANSACTION_REFERENCE_EXPONENT	0x9F3D
#define TAG_AIP								0x82
#define TAG_AFL								0x94
#define TAG_COMMAND_TEMPLATE				0x83
#define TAG_RESPONSE_FORMAT_1				0x80
#define TAG_RESPONSE_FORMAT_2				0x77
#define TAG_READ_RECORD_RESPONSE_TEMPLATE	0x70
#define TAG_APPLICATION_EXP_DATE			0x5F24
#define TAG_PAN								0x5A
#define TAG_CDOL_1							0x8C
#define TAG_CDOL_2							0x8D
#endif