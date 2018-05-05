package ble.lss.com.jnihelper;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity implements TestListener{
    static {
        System.loadLibrary("testlib");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        testStrfunc("hello world!",1024);

        Log.e("JNIJAVA",">>>getString From native:"+getString());
        Log.e("JNIJAVA",">>>getObject From native:"+getObject());

        testCallJava();

        testCallStaticJava();

        testListener(this);
    }

    @Override
    public void onTestCallback(String str, int ivalue) {
        Log.e("JNIJAVA",">>>onTestCallback From native:"+str+" ivalue:"+ivalue);
    }

    static void onStaticFromNative(String str, int ivalue){
        Log.e("JNIJAVA",">>>onStaticFromNative From native:"+str+" ivalue:"+ivalue);
    }
    void onFromNative(String str,int ivalue){
        Log.e("JNIJAVA",">>>onFromNative From native:"+str+" ivalue:"+ivalue);
    }


    native void testStrfunc(String str,int ivalue);
    native String getString();
    native TestObject getObject();
    native void testCallJava();
    native void testCallStaticJava();
    native void testListener(TestListener listener);
}
