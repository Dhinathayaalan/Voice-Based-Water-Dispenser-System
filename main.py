from esp32_assistant import ESP32Assistant
from google.assistant.library import Assistant
from google.assistant.library.event import EventType

esp = ESP32Assistant("192.168.1.50")  # ESP32 IP

def handle_voice(command):
    command = command.lower()
    if 'hot' in command and 'on' in command:
        esp.hot_on()
    elif 'hot' in command and 'off' in command:
        esp.hot_off()
    elif 'cold' in command and 'on' in command:
        esp.cold_on()
    elif 'cold' in command and 'off' in command:
        esp.cold_off()

def main():
    with Assistant() as assistant:
        for event in assistant.start():
            if event.type == EventType.ON_RECOGNIZING_SPEECH_FINISHED and event.args:
                text = event.args['text']
                print(f"Recognized: {text}")
                handle_voice(text)
