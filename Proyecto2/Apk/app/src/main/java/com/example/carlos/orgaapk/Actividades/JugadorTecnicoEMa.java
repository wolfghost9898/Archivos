package com.example.carlos.orgaapk.Actividades;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
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
import com.example.carlos.orgaapk.Adaptadores.AdaptadorJTE;
import com.example.carlos.orgaapk.Adaptadores.AdaptadorJTEMA;
import com.example.carlos.orgaapk.Constantes;
import com.example.carlos.orgaapk.Modelos.ModeloJTE;
import com.example.carlos.orgaapk.Modelos.ModeloJTEMA;
import com.example.carlos.orgaapk.R;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link JugadorTecnicoEMa#newInstance} factory method to
 * create an instance of this fragment.
 */
public class JugadorTecnicoEMa extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    String url=Constantes.ip+"JugadorDEA/";
    Button enviar;
    EditText edad;
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private List<ModeloJTEMA> listNoticias;
    private RecyclerView recyclerView;

    public JugadorTecnicoEMa() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment JugadorTecnicoEMa.
     */
    // TODO: Rename and change types and number of parameters
    public static JugadorTecnicoEMa newInstance(String param1, String param2) {
        JugadorTecnicoEMa fragment = new JugadorTecnicoEMa();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        getActivity().setTitle("Jugadores Tecnico por Edad");
        View view= inflater.inflate(R.layout.fragment_jugador_tecnico_ema, container, false);
        edad=view.findViewById(R.id.edadJTMa);
        enviar=view.findViewById(R.id.enviarJTEMA);
        recyclerView=view.findViewById(R.id.RCJTEMa);
        enviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listNoticias=new ArrayList<>();
                extraerDatos();
            }
        });
        return view;
    }


    void extraerDatos(){
        RequestQueue queue = Volley.newRequestQueue(getContext());
        StringRequest sr = new StringRequest(Request.Method.POST,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONArray jsonArray=new JSONArray(response);
                    for(int i=0;i<jsonArray.length();i++){
                        JSONObject object=jsonArray.getJSONObject(i);
                        String edad=object.getString("edad");
                        String nombre=object.getString("nombre");
                        String tipo=object.getString("tipo");

                        ModeloJTEMA noticias= new ModeloJTEMA(
                                nombre,tipo,edad
                        );
                        listNoticias.add(noticias);


                    }
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                setupRecyclerView(listNoticias);
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                NetworkResponse networkResponse=error.networkResponse;
                if(networkResponse!=null && networkResponse.statusCode==404){
                    Toast.makeText(getContext(),"Problema su busqueda",Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(getContext(),error.getMessage(),Toast.LENGTH_SHORT).show();
                }
            }
        }){
            @Override
            protected Map<String,String> getParams(){
                Map<String,String> params = new HashMap<String, String>();
                params.put("edad",edad.getText().toString());
                return params;
            }

        };
        queue.add(sr);

    }

    private  void setupRecyclerView(List<ModeloJTEMA> listNoticias){
        AdaptadorJTEMA noticiaAdaptador=new AdaptadorJTEMA(getContext(),this.listNoticias);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        recyclerView.setAdapter(noticiaAdaptador);
    }
}
