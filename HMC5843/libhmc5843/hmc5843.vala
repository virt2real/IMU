/* hmc5843.vala
 *
 * Copyright (C) 2010  Michele Brocco
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 *      Michele Brocco <ssj2micvm@gmail.com>
 */


using GLib;

public class HMC5843.Access : GLib.Object {

    private double _x_value;
    private double _y_value;
    private double _z_value;
    private double _counts_per_mgauss;

    //Default Constructor needed for some reason when converting in C code
    public Access(){}

    public double x_value {
		get {
			return this._x_value;
		}
		private set {
			this._x_value = value;
		}

	}
 
    public double counts_per_mgauss {
		get {
			return this._counts_per_mgauss;
		}
		private set {
			this._counts_per_mgauss = value;
		}

	}



    public double y_value {
		get {
			return this._y_value;
		}
		private set {
			this._y_value = value;
		}

	}

    public double z_value {
		get {
			return this._z_value;
		}
		private set {
			this._z_value = value;
		}

	}

      public void readXYZ(){
    	string filename = "/sys/bus/i2c/devices/0-001e/value";
	string content;
	ulong len;
	try{
		//Try to read the file and write the content in the string content
		FileUtils.get_contents (filename, out content, out len);
		//stdout.printf("%s",content);
		int i=0;
		StringBuilder xvalue=new StringBuilder();
		StringBuilder yvalue=new StringBuilder();
		StringBuilder zvalue=new StringBuilder();

		//Parse X Value
		while(content[i]!=' '){
			xvalue.append_unichar(content[i]);
			i++;
		}
		i++;
		//Parse Y Value
		while(content[i]!=' '){
			yvalue.append_unichar(content[i]);
			i++;
		}
		i++;
		//Parse Z Value and test for end of string to terminate
		while((content[i]!=' ')&&(content[i]!=0)){
			zvalue.append_unichar(content[i]);
			i++;
		}
		x_value=xvalue.str.to_double();
		y_value=yvalue.str.to_double();
		z_value=zvalue.str.to_double();
	} catch (Error e){
		
		error("I2C Kernel Interface not found! Did you launch the kernel module hmc5843?");
	}
    }

      public void readCountsPermGauss(){
    	string filename = "/sys/bus/i2c/devices/0-001e/counts_per_mgauss";
	string content;
	ulong len;
	try{
		//Try to read the file and write the content in the string content
		FileUtils.get_contents (filename, out content, out len);
		int i=0;
		StringBuilder cpmg=new StringBuilder();

		//Parse MGausValue Value
		while((content[i]!=' ')&&(content[i]!=0)){
			cpmg.append_unichar(content[i]);
			i++;
		}
		counts_per_mgauss=cpmg.str.to_double();
	}catch(Error e){
		error("I2C Kernel Interface for counter_per_mgauss not found! Is the kernel module hmc5843 loaded?");
	}
    }
    

    public void readSensorData() {
	readXYZ();
	readCountsPermGauss();
	stdout.printf("X is: %.0f, Y is: %.0f, Z is: %.0f\n",x_value,y_value,z_value);
	stdout.printf("Counts per mGauss: %.0f\n",counts_per_mgauss);

        
    }
}
