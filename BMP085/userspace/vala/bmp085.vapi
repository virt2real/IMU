/* bmp085.vapi
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


[CCode (cheader_filename = "bmp085.h")]
namespace Bmp {
	[CCode(cname = "struct bmp085_calibration_data")]
	public struct BmpCalibrationData {
		[CCode(cname = "AC1")]
		public int16 AC1 ;
		[CCode(cname = "AC2")]
		public int16 AC2 ;
		[CCode(cname = "AC3")]
		public int16 AC3 ;
		[CCode(cname = "AC4")]
		public uint16 AC4 ;
		[CCode(cname = "AC5")]
		public uint16 AC5 ;
		[CCode(cname = "AC6")]
		public uint16 AC6 ;
		[CCode(cname = "B1")]
		public int16 B1 ;
		[CCode(cname = "B2")]
		public int16 B2 ;
		[CCode(cname = "MB")]
		public int16 MB ;
		[CCode(cname = "MC")]
		public int16 MC ;
		[CCode(cname = "MD")]
		public int16 MD ;
	}

	[CCode(cname = "struct bmp085_data")]
	public struct BmpData {
		[CCode(cname = "client")]
		public int client;

		[CCode(cname = "calibration")]
		public BmpCalibrationData calibration_data;
		
		[CCode(cname = "raw_temperature")]
		public ulong raw_temperature;

		[CCode(cname = "raw_pressure")]
		public ulong raw_pressure;
		
		[CCode(cname = "oversampling_setting")]
		public uchar oversampling_setting;

		[CCode(cname = "next_temp_measurement")]
		public ulong next_temp_measurement;

		[CCode(cname = "b6")]
		public long calc_correction_coeff;
	}

    	[CCode (cname = "open_bmp085")]
	public int open_bmp085(int adapter_nr, out BmpData data);
    
	[CCode (cname = "close_bmp085")]
	public void close_bmp085 (BmpData data);
	
	[CCode (cname = "set_oversampling")]
	public void set_oversampling (out BmpData data,uchar oversampling);

	[CCode (cname = "get_oversampling")]
	public uchar get_oversampling (BmpData data);

	[CCode (cname = "get_temperature")]
	public int32 get_temperature (BmpData data);

	[CCode (cname = "get_pressure")]
	public int32 get_pressure (BmpData data);

}

