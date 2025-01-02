#include "BluetoothA2DPSink.h"

String input_string;
int i;
unsigned long tmrConnect;

BluetoothA2DPSink a2dp_sink;

void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  if (id == ESP_AVRC_MD_ATTR_TITLE) {
    Serial.printf("Название - %s\n", text);
  } else if (id == ESP_AVRC_MD_ATTR_ARTIST) {
    Serial.printf("Исполнитель - %s\n", text);
  } else if (id == ESP_AVRC_MD_ATTR_PLAYING_TIME) {
    uint32_t playtime = String((char *)text).toInt();
    Serial.printf("Время песни %d секунд\n", (int)round(playtime / 1000.0));
  }
}

void avrc_rn_playstatus_callback(esp_avrc_playback_stat_t playback) {
  switch (playback) {
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_STOPPED:
      Serial.println("Stopped.");
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PLAYING:
      Serial.println("Playing.");
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PAUSED:
      Serial.println("Paused.");
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_FWD_SEEK:
      Serial.println("Forward seek.");
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_REV_SEEK:
      Serial.println("Reverse seek.");
      break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_ERROR:
      Serial.println("Error.");
      break;
    default:
      Serial.printf("Got unknown playback status %d\n", playback);
  }
}

void setup() {
  Serial.begin(115200);
/*   static const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 44100,
    .bits_per_sample = (i2s_bits_per_sample_t)16,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
    .intr_alloc_flags = 0, 
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  }; */

  i2s_pin_config_t my_pin_config = {
    .mck_io_num = I2S_PIN_NO_CHANGE,
    .bck_io_num = 27,    // BCK
    .ws_io_num = 26,     // LRCK
    .data_out_num = 25,  // DIN
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  a2dp_sink.set_avrc_rn_playstatus_callback(avrc_rn_playstatus_callback);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_PLAYING_TIME);
  //a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.set_pin_config(my_pin_config);
  a2dp_sink.start("ESP32_BT_Music");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '-') {
      Command();
      input_string = "";
    } else {
      input_string += c;
    }
  }
  AutoConnect();
}

void Command() {
  if (input_string.equals("1") == true) {  // Плей
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      Serial.println("play");
      a2dp_sink.play();
    }
  } else if (input_string.equals("2") == true) {  // Пауза
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      Serial.println("pause");
      a2dp_sink.pause();
    }
  } else if (input_string.equals("3") == true) {  // Трек вперед
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      Serial.println("next");
      a2dp_sink.next();
    }
  } else if (input_string.equals("4") == true) {  // Трек назад
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      Serial.println("previous");
      a2dp_sink.previous();
    }
  } else if (input_string.equals("5") == true) {  // Узнать уровень громкости
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      Serial.println(a2dp_sink.get_volume());
    }
  } else if (input_string.equals("6") == true) {  // Увеличить громкость
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      if (a2dp_sink.get_volume() <= 119) {
        i = a2dp_sink.get_volume();
        i = i + 8;
        a2dp_sink.set_volume(i);
        Serial.println(i);
      } else if (a2dp_sink.get_volume() > 119) {
        a2dp_sink.set_volume(127);
        Serial.println(a2dp_sink.get_volume());
      }
    }
  } else if (input_string.equals("7") == true) {  // Уменьшить громкость
    if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      if (a2dp_sink.get_volume() >= 8) {
        i = a2dp_sink.get_volume();
        i = i - 8;
        a2dp_sink.set_volume(i);
        Serial.println(i);
      } else if (a2dp_sink.get_volume() < 8) {
        a2dp_sink.set_volume(0);
        Serial.println(a2dp_sink.get_volume());
      }
    }
  }
}

void AutoConnect() {
  if (a2dp_sink.get_connection_state() == 0) {
    if (millis() - tmrConnect >= 1000) {
      tmrConnect = millis();
      a2dp_sink.reconnect();
    }
  }
}
