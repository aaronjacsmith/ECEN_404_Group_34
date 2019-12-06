package com.example.jampak11;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.Activity;
import android.os.AsyncTask;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.ToggleButton;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends Activity implements View.OnClickListener {



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /* Switch Declaration */
        Switch FX1 = (Switch) findViewById(R.id.FX1);
        Switch FX2 = (Switch) findViewById(R.id.FX2);
        Switch FX3 = (Switch) findViewById(R.id.FX3);
        Switch FX4 = (Switch) findViewById(R.id.FX4);

        /*Button Declaration */
        Button buttonPlusdistfuzz = (Button)findViewById(R.id.buttonPlusdistfuzz);
        Button buttonMinusdistfuzz = (Button)findViewById(R.id.buttonMinusdistfuzz);
        Button buttonMinusDelay = (Button)findViewById(R.id.buttonMinusDelay);
        Button buttonPlusDelay = (Button)findViewById(R.id.buttonPlusDelay);

        /*Set button on click activities*/
        buttonMinusdistfuzz.setOnClickListener(this);
        buttonPlusdistfuzz.setOnClickListener(this);
        buttonMinusDelay.setOnClickListener(this);
        buttonPlusDelay.setOnClickListener(this);

//on switch listeners (activates and deactivates the effects)
        FX1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {

                    new Background_get().execute("Dist=1");
                } else {
                    new Background_get().execute("Dist=0");
                }
            }
        });

        FX2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {

                    new Background_get().execute("Fuzz=1");
                } else {
                    new Background_get().execute("Fuzz=0");
                }
            }
        });

        FX3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {

                    new Background_get().execute("Delay=1");
                } else {
                    new Background_get().execute("Delay=0");
                }
            }
        });

        FX4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {

                    new Background_get().execute("Trem=1");
                } else {
                    new Background_get().execute("Trem=0");
                }
            }
        });


    }


//switch statement for button presses (reads the "this" instance as whichever button view is pressed)
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.buttonMinusdistfuzz:
                new Background_get().execute("MinusDistFuzz=1");
                break;
            case R.id.buttonPlusdistfuzz:
                new Background_get().execute("PlusDistFuzz=1");
                break;
            case R.id.buttonMinusDelay:
                new Background_get().execute("MinusDelay=1");
                break;
            case R.id.buttonPlusDelay:
                new Background_get().execute("PlusDelay=1");
                break;
        }
    }


//connection protocol
    private class Background_get extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... params) {
            try {

                URL url = new URL("http://172.20.10.12/?" + params[0]);
                HttpURLConnection connection = (HttpURLConnection) url.openConnection();

                BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                StringBuilder result = new StringBuilder();
                String inputLine;
                while ((inputLine = in.readLine()) != null)
                    result.append(inputLine).append("\n");

                in.close();
                connection.disconnect();
                return result.toString();

            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
    }

}


