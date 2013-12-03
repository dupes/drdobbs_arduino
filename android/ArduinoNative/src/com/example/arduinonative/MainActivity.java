package com.example.arduinonative;

import java.util.ArrayList;
import java.util.HashMap;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONObject;

import android.app.Activity;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class MainActivity extends Activity {

	private static final String TAG = "MAIN_ACTIVITY";
	
	private EditText m_edtIPAddress;
	private ListView m_listView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();

		StrictMode.setThreadPolicy(policy); 
		
		Button btnRead = (Button)findViewById(R.id.btnRead);
		
		m_listView = (ListView)findViewById(R.id.listView);
		m_edtIPAddress = (EditText)findViewById(R.id.edtURL);
		
		btnRead.setOnClickListener(new View.OnClickListener() {
	         public void onClick(View v) {
	        	 String jsonString = readTemperature();
	        	 
	        	 if (jsonString == null)
	        		 return;
	        	 
	        	 loadTemperature(jsonString);
	         }

		});
	}
	
	/****************************************************************************/
	
	/**
	 * load the temperature data from a json string to the ListView
	 * @param jsonString
	 */
	private void loadTemperature(String jsonString) {
		try
		{
			JSONArray jsonArray = new JSONArray(jsonString);
			ArrayList<HashMap<String, String>> items = new ArrayList<HashMap<String, String>>();
			HashMap<String, String> map;
			SimpleAdapter simpleAdapter;
			
			for (int index = 0; index < jsonArray.length(); index++) {
				JSONObject jsonObject = jsonArray.getJSONObject(index);
				
				map = new HashMap<String, String>();
				
				map.put("id", jsonObject.getString("id"));
				map.put("celsius", "celsius: " + jsonObject.getString("celsius"));
				map.put("fahrenheit", "fahrenheit: " + jsonObject.getString("fahrenheit"));
				
				items.add(map);
			}
			
			simpleAdapter = new SimpleAdapter(this, items, R.layout.row, new String[]{"id", "celsius", "fahrenheit"}, new int[] {R.id.tvID, R.id.tvCelsius, R.id.tvFahrenheit});
			m_listView.setAdapter(simpleAdapter);
		}
		catch (Exception ex)
		{
			Log.d(TAG, "loadTemperature exception: " + ex.getMessage());
			
			Toast.makeText(this, "loadTemperature exception: " + ex.getMessage(), Toast.LENGTH_LONG).show();
		}		
	}
	
	/****************************************************************************/
	
	private String readTemperature() {
        
		try
		{
			String ipAddress = m_edtIPAddress.getText().toString();		
			
	        HttpClient httpclient = new DefaultHttpClient();	        
	        
	        HttpGet httpget = new HttpGet(ipAddress);
	        
			HttpResponse httpResponse = httpclient.execute(httpget);
			
			String response = ConvertInputStream.inputStreamToString(httpResponse.getEntity().getContent());
			
			Toast.makeText(this, "load complete", Toast.LENGTH_LONG).show();
			
			return response;
		}
		catch (Exception ex)
		{
			Log.d(TAG, "readTemperature exception: " + ex.getMessage() + ex.toString());
			
			ex.printStackTrace();
			
			Toast.makeText(this, "readTemperature exception: " + ex.getMessage(), Toast.LENGTH_SHORT).show();
		}
		
		return null;
	}
	
	/****************************************************************************/
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
