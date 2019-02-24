package com.example.carlos.orgaapk.Actividades;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.carlos.orgaapk.Actividades.Registrar;
import com.example.carlos.orgaapk.Constantes;
import com.example.carlos.orgaapk.MainActivity;
import com.example.carlos.orgaapk.R;
import com.example.carlos.orgaapk.UserSessionManager;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


import java.util.HashMap;
import java.util.Map;



public class Logic extends AppCompatActivity {

    UserSessionManager session;


 EditText usuario,contrasenia;
 Context context;
 Button login,crearUsuario;
 SharedPreferences pref;
 Intent intent;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_logic);
        context=this;

        usuario=(EditText)findViewById(R.id.usuarioLogin);
        contrasenia=(EditText)findViewById(R.id.contraseniaLogin);
        login=(Button)findViewById(R.id.loginUsuario);
        crearUsuario=(Button)findViewById(R.id.botonRegister);

        //---------------------------------------------crear usuario---------------------------------------------------------

        crearUsuario.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Intent in=new Intent(getApplicationContext(),Registrar.class);
                in.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                in.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                getApplicationContext().startActivity(in);

            }
        });


        session=new UserSessionManager(getApplicationContext());



        //--------------------------------Boton de login----------------------------------
        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String url=Constantes.ip+"UsuarioAndroid/?pk="+usuario.getText().toString()+"&contra="+contrasenia.getText().toString();
               getLogin(url);
            }
        });
    }




    void getLogin(String url){
        RequestQueue queue = Volley.newRequestQueue(getApplicationContext());
        StringRequest sr = new StringRequest(Request.Method.GET,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONArray jsonArray=new JSONArray(response);
                    for(int i=0;i<jsonArray.length();i++){
                        JSONObject object=jsonArray.getJSONObject(i);
                        String nombre=object.getString("Nombre");
                        String usuario=object.getString("Usuario");
                        String membresia=object.getString("Membresia");
                        int tipo=object.getInt("Tipo");
                        String tempTipo=String.valueOf(tipo);
                        session.createUserLoginSession(usuario,tempTipo,membresia);
                        Intent in=new Intent(getApplicationContext(),MainActivity.class);
                        in.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        in.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        getApplicationContext().startActivity(in);
                        finish();

                    }
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                NetworkResponse networkResponse=error.networkResponse;
                if(networkResponse!=null && networkResponse.statusCode==404){
                    Toast.makeText(context,"Usuario o COntraseña erroneas",Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(context,error.getMessage(),Toast.LENGTH_SHORT).show();
                }
            }
        }){
            @Override
            protected Map<String,String> getParams(){
                Map<String,String> params = new HashMap<String, String>();
                return params;
            }

        };
        queue.add(sr);
    }















}
