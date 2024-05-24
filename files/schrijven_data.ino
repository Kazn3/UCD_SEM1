#include <SPI.h>
#include <MFRC522.h>

/*Using Hardware SPI of Arduino */
/*MOSI (11), MISO (12) and SCK (13) are fixed */
/*You can configure SS and RST Pins*/
#define SS_PIN 10  /* Slave Select Pin */
#define RST_PIN 9  /* Reset Pin */

/* Create an instance of MFRC522 */
MFRC522 mfrc522(SS_PIN, RST_PIN);
/* Create an instance of MIFARE_Key */
MFRC522::MIFARE_Key key;          

/* Set the block to which we want to write data */
/* Be aware of Sector Trailer Blocks */
int blockNum = 2;  
/* Create an array of 16 Bytes and fill it with data */
/* This is the actual data which is going to be written into the card */
byte blockData [16] = {"schrijftest"};


MFRC522::StatusCode status;



void setup() 
{
  /* Initialize serial communications with the PC */
  Serial.begin(9600);
  /* Initialize SPI bus */
  SPI.begin();
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  Serial.println("start_programma_schrijven");
}

void loop()
{      
   /* Prepare the ksy for authentication */
  /* All keys are set to FFFFFFFFFFFFh at chip delivery from the factory */
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  /* Call 'WriteDataToBlock' function, which will write data to the block */
  Serial.print("\n");
  Serial.println("Writing to Data Block...");
  WriteDataToBlock(blockNum, blockData);
  delay(1000);  
}

void WriteDataToBlock(int blockNum, byte blockData[]) 
 {
   /* Authenticating the desired data block for write access using Key A */
   status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
   if (status != MFRC522::STATUS_OK)
   {
     Serial.print("Authentication failed for Write: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
   }
   else
   {
     Serial.println("Authentication success");
   }

  
   /* Write data to the block */
   status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
   if (status != MFRC522::STATUS_OK)
   {
     Serial.print("Writing to Block failed: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
   }
   else
   {
     Serial.println("Data was written into Block successfully");
   }

 }



