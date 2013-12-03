package com.example.arduinonative;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class ConvertInputStream {

	public static String inputStreamToString(InputStream inputStream) throws IOException {
		
		BufferedReader bufferReader = new BufferedReader(new InputStreamReader(inputStream));
		StringBuilder stringBuilder = new StringBuilder();
		
		String line;
		
		while ((line = bufferReader.readLine()) != null)
			stringBuilder.append(line);
		
		return stringBuilder.toString();
	}
}
