using GLib;

public class TestApp : GLib.Object {
    public static int main(string[] args) {
        HMC5843.Access test = new HMC5843.Access();

        test.readSensorData();

        return 0;
    }
}
