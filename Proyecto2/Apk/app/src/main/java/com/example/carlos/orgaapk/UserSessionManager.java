package com.example.carlos.orgaapk;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import com.example.carlos.orgaapk.Actividades.Logic;

import java.util.HashMap;

public class UserSessionManager {
    SharedPreferences pref;
    SharedPreferences.Editor editor;
    Context _context;
    int PRIVATE_MODE=0;

    private static final String PREFER_NAME="Usuario";
    private static final String IS_USER_LOGIN="IsUserLoggedIn";
    public static final String KEY_NAME="usuario";
    public static final String KEY_TIPO="tipo";
    public static final String KEY_MEMBRESIA="membresia";

    public UserSessionManager(Context context){
        this._context=context;
        pref=_context.getSharedPreferences(PREFER_NAME,PRIVATE_MODE);
        editor=pref.edit();
    }

    public void createUserLoginSession(String name,String tipo,String membresia){
        editor.putBoolean(IS_USER_LOGIN,true);
        editor.putString(KEY_NAME,name);
        editor.putString(KEY_TIPO,tipo);
        editor.putString(KEY_MEMBRESIA,membresia);
        editor.commit();
    }

    public void editarMembresia(String membresia){
        editor.putString(KEY_MEMBRESIA,membresia);
        editor.commit();
    }

    public boolean isUserLoggedIn(){
        return pref.getBoolean(IS_USER_LOGIN, false);
    }

    public boolean checkLogin(){
        if(!this.isUserLoggedIn()){
            Intent i=new Intent(_context,Logic.class);
            i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            _context.startActivity(i);
            return true;
        }
        return false;
    }


    public HashMap<String,String> getUserDetails(){
        HashMap<String,String> user=new HashMap<String, String>();
        user.put(KEY_NAME,pref.getString(KEY_NAME,null));
        user.put(KEY_TIPO,pref.getString(KEY_TIPO,null));
        user.put(KEY_MEMBRESIA,pref.getString(KEY_MEMBRESIA,null));
        return user;
    }

    public void LogoutUser(){
        editor.clear();
        editor.commit();

        Intent i= new Intent(_context,Logic.class);
        i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        _context.startActivity(i);
    }
}
