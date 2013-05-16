using GLib;
using Bmp;

public class Hello : GLib.Object {
    public static int main(string[] args) {
	BmpData data; 
	open_bmp085(0,out data);
	int temp=get_temperature(data);
	int pressure=get_pressure(data);

	stdout.printf("Pressure: %fhPa\tt:, Temperature: %fC\n", pressure/100.0, temp/10.0);
	close_bmp085(data);


        return 0;
    }
}
