package com.example.arduinowebview;

import android.app.Activity;
import android.os.Build;
import android.os.Build.VERSION_CODES;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.Menu;
import android.webkit.WebView;

public class MainActivity extends Activity {

	static final String TAG = MainActivity.class.getName();
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        // allow http query in main thread (this is not recommended) 
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy); 
                
        WebView webView = (WebView)findViewById(R.id.webView1);
        
        webView.getSettings().setJavaScriptEnabled(true);
        
        WebInterface webInterface = new WebInterface(this);
        
        webView.addJavascriptInterface(webInterface, "WebInterface");
                
        webView.setWebChromeClient(webInterface);
        
        webView.loadUrl("file:///android_asset/temperature.html");
                
        Log.d(TAG, "complete");
    }
    
    /****************************************************************************/
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
