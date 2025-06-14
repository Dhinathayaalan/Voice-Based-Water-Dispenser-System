import requests

class ESP32Assistant:
    def __init__(self, ip_address):
        self.base_url = f"http://{ip_address}"

    def send_command(self, device: str, action: str):
        """
        device: 'hot', 'cold', or 'led'
        action: 'on' or 'off'
        """
        endpoint = f"/{device}_{action}"
        try:
            r = requests.get(self.base_url + endpoint, timeout=3)
            return r.status_code == 200
        except Exception as e:
            print(f"Error sending command: {e}")
            return False

    def hot_on(self): return self.send_command("hot", "on")
    def hot_off(self): return self.send_command("hot", "off")
    def cold_on(self): return self.send_command("cold", "on")
    def cold_off(self): return self.send_command("cold", "off")
