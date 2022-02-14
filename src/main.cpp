#include "..\include\main.h"

void TASK1_MAV_READ_TCP_CHANNEL_0(void *param)
{
  mavlink_status_t tcp_status; // Updated struct that stores the status of the current message being parsed for that channel
  mavlink_message_t tcp_msg;   // Struct that stores the contents of the decoded message
  int tcp_chan = MAVLINK_COMM_0;

  while (1)
  {

    while (!client.connect("sorair.mywire.org", 6544)) // Connect to host
    {
      if (DEBUG)
      {
        Serial.println("DEBUG: Connection to host failed");
      }

      vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    while (client.connected()) // If client is connected, Run infinitly
    {

      if (mavlink_parse_char(tcp_chan, client.read(), &tcp_msg, &tcp_status)) // Returns 1 if was successful
      {

        MAV_MSG_ENCODER msg_encoder; // Create a message encoder container that will hold rtn buffer

        if (MAV_PARSER::MAV_MSG_PARSER(&tcp_msg, &msg_encoder))
        {
          client.write(msg_encoder.rtn_buffer, msg_encoder.rtn_buffer_len);
        }
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

        if (MAV_PARSER::MAV_MSG_PARSER(&serial_msg, &msg_encoder))
        {
          if (xSemaphoreTake(serial_channel_1_mutex, 1 / portTICK_PERIOD_MS) == pdTRUE) // Block for 1ms if cannot take
          {
            Serial.write(msg_encoder.rtn_buffer, msg_encoder.rtn_buffer_len);

            xSemaphoreGive(serial_channel_1_mutex); // Give back
          }
        }
      }
    }
  }
};

void TIMER1_MAV_SEND_HEARTBEAT(TimerHandle_t xtimer)
{
  MAV_MSG_ENCODER msg_encoder;
  msg_encoder.encode_heartbeat();

  if (xSemaphoreTake(serial_channel_1_mutex, 0.1 / portTICK_PERIOD_MS) == pdTRUE)
  {
    Serial.write(msg_encoder.rtn_buffer, msg_encoder.rtn_buffer_len);

    xSemaphoreGive(serial_channel_1_mutex);
  }

  if (client.connected())
    client.write(msg_encoder.rtn_buffer, msg_encoder.rtn_buffer_len);
}

void setup()
{
  Serial.begin(115200);
  COM.BEGIN(VSPI);

  PILOT_STATE = MAV_STATE_BOOT;

  pinMode(2, OUTPUT);

  serial_channel_1_mutex = xSemaphoreCreateMutex(); // Create mutex befre staring all task

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
  else
  {
    if (DEBUG)
      Serial.println("DEBUG: Heartbeat timer failed. Restarting Autopilot....");
    ESP.restart();
  }

  if (ENABLE_TCP_CHANNEL)
  {
    TCP.INO_WIFI->begin("BT-MQAHXC", "PFL7mPdAkrc4KX");

    while (TCP.INO_WIFI->status() != WL_CONNECTED)
    {
      vTaskDelay(500 / portTICK_PERIOD_MS);
      if (DEBUG)
        Serial.println("Waiting for connection");
    }

    if (DEBUG)
    {
      Serial.println("Connected to WIFI");

      Serial.println(TCP.INO_WIFI->localIP());
    }

    xTaskCreatePinnedToCore(TASK1_MAV_READ_TCP_CHANNEL_0,
                            "Reads Mavlink message from TCP Port",
                            5000,
                            NULL,
                            1,
                            &task_1_mav_tcp,
                            app_cpu);
  }

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
