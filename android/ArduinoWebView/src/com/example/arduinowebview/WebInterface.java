package com.example.arduinowebview;

import android.content.Context;

import android.util.Log;
import android.widget.Toast;

import android.webkit.*;

public class WebInterface extends WebChromeClient {
	private Context m_context;
	private static final String TAG = WebInterface.class.getName();
	
	WebInterface(Context context) {
		m_context = context;
	}
	
	@JavascriptInterface
    public void logMessage(String message) {
    	
		Log.d(TAG, message);
		
    	Toast.makeText(m_context, message, Toast.LENGTH_SHORT).show();
    }
	
	/*
	 * (non-Javadoc)
	 * @see android.webkit.WebChromeClient#onJsAlert(android.webkit.WebView, java.lang.String, java.lang.String, android.webkit.JsResult)
	 */
    public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
        result.confirm();
        Toast.makeText(m_context, message, Toast.LENGTH_LONG).show();
        
        return true;
    }
}
