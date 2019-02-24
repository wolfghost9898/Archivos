package com.example.carlos.orgaapk.Actividades;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.carlos.orgaapk.Constantes;
import com.example.carlos.orgaapk.R;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Registrar extends AppCompatActivity {

    Context context;

    ArrayList<String> Genero;
    ArrayList<String> idGenero;

    EditText nombre,apellido,correo,usuario,contrasenia,telefono,direccion,fecha;
    Button imagen,guardar;
    Spinner genero;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registrar);

        context=this;

        Genero=new ArrayList<>();
        idGenero=new ArrayList<>();

        Genero.add("Masculino");
        idGenero.add("M");
        Genero.add(("Femenino"));
        idGenero.add("F");


        nombre=(EditText)findViewById(R.id.nombreRegistro);
        apellido=(EditText)findViewById(R.id.apellidoRegistro);
        correo=(EditText)findViewById(R.id.correoRegistro);
        usuario=(EditText)findViewById(R.id.usuarioRegistro);
        contrasenia=(EditText)findViewById(R.id.contraseñaRegistro);
        telefono=(EditText)findViewById(R.id.numeroRegistro);
        direccion=(EditText)findViewById(R.id.direccionRegistro);
        fecha=(EditText)findViewById(R.id.fechaRegistro);

        genero=(Spinner)findViewById(R.id.generoRegistro);

        genero.setAdapter(new ArrayAdapter<String>(getApplicationContext(),android.R.layout.simple_spinner_dropdown_item,Genero));


        guardar=(Button)findViewById(R.id.guardarRegistro);




        guardar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String url=Constantes.ip+"guardarUsuarioAndroid/";
                crearUnUsuario(url);
            }
        });

    }



   private void crearUnUsuario(String url){
        RequestQueue queue = Volley.newRequestQueue(getApplicationContext());
        StringRequest sr = new StringRequest(Request.Method.POST,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONObject object=new JSONObject(response);
                    String error=object.getString("Error");
                    if(error.equals("0")) {
                        Toast.makeText(context, "Usuario creado con exito, por favor revise su correo", Toast.LENGTH_SHORT).show();
                        usuario.setText("");
                        apellido.setText("");
                        contrasenia.setText("");
                        nombre.setText("");
                        telefono.setText("");
                        direccion.setText("");
                        fecha.setText("");
                    }else{
                        Toast.makeText(context,"Error: Este usuario ya existe",Toast.LENGTH_SHORT).show();
                    }

                } catch (JSONException e) {
                    e.printStackTrace();
                    Toast.makeText(context,e.getMessage(),Toast.LENGTH_SHORT).show();

                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                Toast.makeText(context,error.getMessage(),Toast.LENGTH_SHORT).show();

            }
        }){
            @Override
            protected Map<String,String> getParams(){
                Map<String,String> params = new HashMap<String, String>();
                params.put("contrasenia",contrasenia.getText().toString());
                params.put("genero",idGenero.get(genero.getSelectedItemPosition()));
                params.put("apellido",apellido.getText().toString());
                params.put("usuario",usuario.getText().toString());
                params.put("fecha",fecha.getText().toString());
                params.put("numero",telefono.getText().toString());
                params.put("direccion",direccion.getText().toString());
                params.put("correo",correo.getText().toString());
                params.put("nombre",nombre.getText().toString());
                return params;
            }

        };
        queue.add(sr);
    }
}
