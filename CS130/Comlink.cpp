#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

typedef unsigned char uchar;

// Hamming encoding packages data and parity bits
//
// Input:  0  0  0  0  d3 d2 d1 d0
// Input:  0  0  0  0  0  p2 p1 p0 
// Output: 0  d3 d2 d1 p2 d0 p1 p0

uchar hamming_encode(uchar d, uchar p) {
	uchar d3, d2, d1, d0, p2, p1, p0;
	d3 = (d >> 3) & 0x1;
	d2 = (d >> 2) & 0x1; 
	d1 = (d >> 1) & 0x1;
	d0 = d & 0x1;
	p2 = (p >> 2) & 0x1;
	p1 = (p >> 1) & 0x1;
	p0 = p & 0x1;
	uchar output = (d3<<6) | (d2<<5) | (d1<<4) | (p2<<3) | (d0<<2) | (p1<<1) | p0;
	
	return output;
}

// Hamming decoding: extract parity bits
//
// Input:  0  d3 d2 d1 p2 d0 p1 p0
// Output: 0  0  0  0  0  p2 p1 p0

uchar hamming_getparity(uchar c) {
	uchar p2, p1, p0;
	p2 = (c >> 3) & 0x1;
	p1 = (c >> 1) & 0x1;
	p0 = c & 0x1;
	uchar output = (p2 << 2) | (p1 << 1) | p0;
	
	return output;
}


// Hamming decoding: extract data bits
//
// Input:  0  d3 d2 d1 p2 d0 p1 p0
// Output: 0  0  0  0  d3 d2 d1 d0

uchar hamming_getdata(uchar d) { 
	uchar d3, d2, d1, d0;
	d3 = (d >> 6) & 0x1;
	d2 = (d >> 5) & 0x1; 
	d1 = (d >> 4) & 0x1;
	d0 = (d >> 2) & 0x1;
	uchar output = (d3 << 3) | (d2 << 2) | (d1 << 1) | d0;

	return output; 
}

// Bad correction calculates which bit in the hamming
// encoded word needs to be corrected by a complement
// operation. The bit index is the sum of the indices 
// of the parity bits which are different from their 
// expected value (ps vs pb below). 
//
// Index:   8  7  6  5  4  3  2  1
// Hamming: 0  d3 d2 d1 p2 d0 p1 p0
// Data:    -  c3 c2 c1 -  c0 -  -
//
// Thus, if no parity bits come out different from their
// expected values, all data bits are good. If, on the
// other hand, say p0 and p2 come out different, then 
// d1 needs to be flipped (1+4=5). That in turn means 
// c1 since the bits have been extracted and repackaged.

uchar badbit_correction(uchar c, uchar pa, uchar pb) { 
	uchar bad = 0;
	if ((pa ^ pb) & 0x1) {
		bad += 0x1;	
	}
	if ((pa ^ pb) & 0x2) { 
		bad += 0x2;
	}
	if ((pa ^ pb) & 0x4) {
		bad += 0x4;
	}
	if (bad == 3) {
		c = (c ^ 0x1);
	}
	if (bad == 5) {
		c = (c ^ 0x2);
	}
	if (bad == 6) {
		c = (c ^ 0x4);
	}
	if (bad == 7) {
		c = (c ^ 0x8);
	}
	return c;
}


int main(int argc, char *argv[]) {

//  declare and fill parity[16] array
//
//  TA, here is my V5 output: 0 3 5 6 1 3 5 7 1 3 5 7 1 2 4 7 


	uchar a0, a1, a2, g0, g1, g2, g3;
	uchar parity[16];
	uchar j = 0;
	for (int i = 0; i <  16; i++) {
		g0 = j & 0x1;
		g1 = (j >> 1) & 0x1;
		g2 = (j >> 2) & 0x1;
		g3 = (j >> 3) & 0x1;
		a0 = g3 ^ g1 ^ g0;
		a1 = (g3 ^ g2 ^ g0) << 1;
		a2 = (g3 ^ g2 ^ g1) << 2;
		parity[i] = a0 | a1 | a2;

		// cout << (int) parity[i] << " ";
		j++;
	}


  if (argc != 2 || strlen(argv[1]) == 0) {
    cerr << "usage: " << argv[0] << " magic_key\n";
    return 0;
  }
  uchar buffer1[128];
  uchar buffer2[256];

 string magic_key = argv[1];
 int NMK = magic_key.length();

if (cin.peek() == 0) { // decrypting
	cin.get(); // skip 0x0 byte
  while (1) {
	  cin.read((char *)buffer2, 256);
	      if ((cin.eof()) && (cin.gcount() == 0))
			    break;
		for (int i = 0; i < cin.gcount()/2; i++) {
			
			uchar firstbyte = buffer2[i * 2];
			uchar secondbyte = buffer2[(i * 2) + 1];

			uchar parity1 = hamming_getparity(firstbyte); // extract parity bits for byte one
			uchar parity2 = hamming_getparity(secondbyte); // extract parity bits for byte two
			uchar data1 = hamming_getdata(firstbyte); // extra data bits for byte one
			uchar data2 = hamming_getdata(secondbyte); // extra data bits for byte two
			
			// correct any bad data bits for byte one
			data1 = badbit_correction(data1, parity1, parity[data1]);

			// correct any bad data bits for byte two
			data2 = badbit_correction(data2, parity2, parity[data2]); 

			buffer1[i] = (data1 << 4) | data2; // form encrypted character from bytes one, two

			buffer1[i] = buffer1[i] ^ magic_key[i % NMK]; // xor-decrypt character
		}
	cout.write((char *)buffer1, cin.gcount() / 2); // write decrypted characted to buffer
  }
}
else { // encrypting
	cout.put(0); // write encryption marker to stdout
  while (1) {
	  cin.read((char *)buffer1, 128);
		if ((cin.eof()) && (cin.gcount() == 0)) 
             break;
		for (int i = 0; i < cin.gcount(); i++) {
			// xor-encrypt character
			buffer1[i] = buffer1[i]	^ magic_key[i % NMK];

			// extract high half-byte  (0x0f)
			uchar firstbyte = (buffer1[i] >> 4) & 0x0F;
			// extract low half-byte  (0x0f)
			uchar secondbyte = buffer1[i] & 0x0F;

		    // determine parity for high half-byte
	        uchar d0, d1, d2, d3, p1, p2, p0, firstparity;
			d0 = firstbyte & 0x1;
			d1 = (firstbyte >> 1) & 0x1;
	        d2 = (firstbyte >> 2) & 0x1;
			d3 = (firstbyte >> 3) & 0x1;
			p0 = d3 ^ d1 ^ d0;
			p1 = (d3 ^ d2 ^ d0) << 1;
			p2 = (d3 ^ d2 ^ d1) << 2;
			firstparity = p0 | p1 | p2;
			
			// determine parity for low half-byte
			uchar b0, b1, b2, b3, q0, q1, q2, secondparity;
			b0 = secondbyte & 0x1;
			b1 = (secondbyte >> 1) & 0x1;
			b2 = (secondbyte >> 2) & 0x1;
			b3 = (secondbyte >> 3) & 0x1;
			q0 = b3 ^ b1 ^ b0;
            q1 = (b3 ^ b2 ^ b0) << 1;
			q2 = (b3 ^ b2 ^ b1) << 2;
			secondparity = q0 | q1 | q2;
		
			// write hamming encoded high half-byte to buffer
			buffer2[i * 2] = hamming_encode(firstbyte,firstparity);
			// write hamming encoded low half-byte to buffer
			buffer2[(i * 2) + 1] = hamming_encode(secondbyte, secondparity);

        }
    cout.write((char *)buffer2, cin.gcount() * 2); // write buffer to stdout
  }
}
}

// Worked with Tyler Abercrombie
