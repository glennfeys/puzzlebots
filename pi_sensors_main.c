#include <stdio.h>

#include "sensors/hts221.h"
#include "sensors/lps25h.h"

int main(int argc, char *argv[]) {
    printf("SENSORS\n");
    
    
    int result = hts221_init();
    if (result < 0) {
		printf("error: initializing hts221 failed\n");
		return(1);
	}
    
    int hts221_humidity = hts221_read_humidity();
    if (hts221_humidity < 0) {
		printf("error: reading hts221 humidity failed\n");
		return(1);
	}
	printf("hts221 humidity: %d%%\n", hts221_humidity);
	int hts221_temperature = hts221_read_temperature();
    if (hts221_temperature < 0) {
		printf("error: reading hts221 temperature failed\n");
		return(1);
	}
	printf("hts221 temperature: %d°C\n", hts221_temperature);
    
    
    result = lps25h_init();
    if (result < 0) {
		printf("error: initializing lps25h failed\n");
		return(1);
	}
    
    int lps25h_pressure = lps25h_read_pressure();
    if (lps25h_pressure < 0) {
		printf("error: reading lps25h pressure failed\n");
		return(1);
	}
	printf("lps25h pressure: %dhPa\n", lps25h_pressure);
	int lps25h_temperature = lps25h_read_temperature();
    if (lps25h_temperature < 0) {
		printf("error: reading lps25h temperature failed\n");
		return(1);
	}
	printf("lps25h temperature: %d°C\n", lps25h_temperature);    
    return 0;
}
