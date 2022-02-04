#include "..\include\main.h"

void TASK1_MAV_READ_TCP_CHANNEL_0(void *param)
{
  mavlink_status_t tcp_status; // Updated struct that stores the status of the current message being parsed for that channel
  mavlink_message_t tcp_msg;   // Struct that stores the contents of the decoded message
  int tcp_chan = MAVLINK_COMM_0;

  while (1)
  {

    while (!client.connect("192.168.1.197", 65432)) // Connect to host
    {
      if (DEBUG)
      {
        Serial.println("DEBUG: Connection to host failed");
      }

      vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 500ms
    }

    while (client.connected()) // If client is connected, Run infinitly
    {
      // uint8_t byte = Serial.read();
      if (mavlink_parse_char(tcp_chan, client.read(), &tcp_msg, &tcp_status)) // Returns 1 if was successful
      {
        // msg = NULL if error
        client.printf("Received message with ID %d, sequence: %d from component %d of system %d\n", tcp_msg.msgid, tcp_msg.seq, tcp_msg.compid, tcp_msg.sysid);
        // ... DECODE THE MESSAGE PAYLOAD HERE ...

        // mavlink_msg_global_position_int_decode()
      }
    }
  }
}

void TASK2_MAV_READ_SERIAL_CHANNEL_1(void *param)
{
  mavlink_status_t serial_status; // Updated struct that stores the status of the current message being parsed for that channel
  mavlink_message_t serial_msg;   // Struct that stores the contents of the decoded message
  int serial_chan = MAVLINK_COMM_1;

  while (1)
  {
    while (Serial.available()) // If client is connected, Run infinitly
    {
      uint8_t byte = Serial.read();
      if (mavlink_parse_char(serial_chan, byte, &serial_msg, &serial_status)) // Returns 1 if was successful
      {

        MAV_MSG_ENCODER msg_encoder; // Create a message encoder container that will hold rtn buffer
        MAV_CMD_PARSER::MAV_PARSER(&serial_msg, &msg_encoder);

        // Serial.write(msg_encoder.rtn_buffer, msg_encoder.rtn_buffer_len);
      }
    }
  }
};

void TIMER1_MAV_SEND_HEARTBEAT(TimerHandle_t xtimer)
{

  MAV_MSG_ENCODER *msg_encoder;
  msg_encoder->encode_heartbeat();
  Serial.write(msg_encoder->rtn_buffer, msg_encoder->rtn_buffer_len);
}

void setup()
{
  Serial.begin(115200);
  COM.BEGIN(VSPI);

  PILOT_STATE = MAV_STATE_BOOT;

  pinMode(2, OUTPUT);

  timer_1_mav_heatbeat = xTimerCreate("Mav Heart-beat timer",    // Name
                                      1000 / portTICK_PERIOD_MS, // Period of timer(in ticks)
                                      pdTRUE,                    // Repeatable
                                      (void *)0,                 // Timer id,
                                      TIMER1_MAV_SEND_HEARTBEAT  // callback func
  );

  if (timer_1_mav_heatbeat != nullptr) // If timer creation was successfull
  {
    xTimerStart(timer_1_mav_heatbeat, portMAX_DELAY); // Start timer
  }

  // TCP.INO_WIFI->begin("BT-MQAHXC", "PFL7mPdAkrc4KX");
  // TCP.INO_WIFI->begin("Ade's-Phone", "Ade4001!");

  // while (TCP.INO_WIFI->status() != WL_CONNECTED)
  // {
  //   vTaskDelay(500 / portTICK_PERIOD_MS);
  //   Serial.println("Waiting for connection");
  // }

  // Serial.println("Connected to WIFI");
  // Serial.println(TCP.INO_WIFI->localIP());

  // xTaskCreatePinnedToCore(TASK1_MAV_READ_TCP_CHANNEL_0,
  //                         "Reads Mavlink message from TCP Port",
  //                         5000,
  //                         NULL,
  //                         1,
  //                         &task_1_mav_tcp,
  //                         app_cpu);

  xTaskCreatePinnedToCore(TASK2_MAV_READ_SERIAL_CHANNEL_1,
                          "Reads Mavlink message from Serial Port",
                          5000,
                          NULL,
                          1,
                          &task_2_mav_serial,
                          app_cpu);
}
void loop()
{
}

void loop10()
{

  // byte sending_data[3] = {1, 2, 3};

  // COM.SPI_TRANSFER(sending_data);

  // for (int i = 0; i < 3; i++)
  // {
  //   Serial.println(COM.rx_buffer[i]);
  // }

  // Serial.println(" ");

  // delay(1000);

  // WiFiClient client;

  // if (!client.connect("192.168.1.197", 65432))
  // {

  //   Serial.println("Connection to host failed");

  //   // vTaskDelay(500 / portTICK_PERIOD_MS);
  //   return;
  // }

  // while (client.connected())
  // {

  //   byte rx[4];

  //   double start = micros();

  // client.read(rx, 4);

  //   double done = micros() - start;

  //   float a; // 4 bytes

  //   memcpy(&a, rx, sizeof(float));

  //   Serial.println(a, 6);

  //   Serial.println(done);

  //   vTaskDelay(500 / portTICK_PERIOD_MS);
  // }

  // client.stop();
}
