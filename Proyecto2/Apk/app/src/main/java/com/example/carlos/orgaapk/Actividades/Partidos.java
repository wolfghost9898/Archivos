package com.example.carlos.orgaapk.Actividades;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.carlos.orgaapk.Adaptadores.NoticiaAdaptador;
import com.example.carlos.orgaapk.Adaptadores.PartidoAdaptador;
import com.example.carlos.orgaapk.Constantes;
import com.example.carlos.orgaapk.Modelos.ModeloPartido;
import com.example.carlos.orgaapk.Modelos.Noticias;
import com.example.carlos.orgaapk.R;
import com.example.carlos.orgaapk.UserSessionManager;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link Partidos.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link Partidos#newInstance} factory method to
 * create an instance of this fragment.
 */
public class Partidos extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    Spinner comboTipo;
    Button enviar;
    ArrayList<String> tipo;

    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    String url=Constantes.ip+"buscarPartidosA/";
    private List<ModeloPartido> listNoticias;
    private RecyclerView recyclerView;

    private OnFragmentInteractionListener mListener;

    public Partidos() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment Partidos.
     */
    // TODO: Rename and change types and number of parameters
    public static Partidos newInstance(String param1, String param2) {
        Partidos fragment = new Partidos();
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

        tipo=new ArrayList<>();

        getActivity().setTitle("Partidos");
        UserSessionManager session=new UserSessionManager(getContext());
        HashMap<String,String> user=session.getUserDetails();
        String tipou=user.get("membresia");

        tipo.add("Sin Iniciar");
        tipo.add("Finalizado");
        tipo.add("Suspendido");
        tipo.add("Todos");
        if (tipou.equals("1"))
            tipo.add("Favoritos");



        View view= inflater.inflate(R.layout.fragment_partidos, container, false);


        recyclerView= view.findViewById(R.id.RCPartido);

        comboTipo=view.findViewById(R.id.spinnerPartido);

        comboTipo.setAdapter(new ArrayAdapter<String>(getContext(),android.R.layout.simple_spinner_dropdown_item,tipo));


        enviar=view.findViewById(R.id.botonPartidos);

        enviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listNoticias=new ArrayList<>();
                cargarPartidos();
            }
        });

        return view;
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);

    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }

    void cargarPartidos(){
        RequestQueue queue = Volley.newRequestQueue(getContext());
        StringRequest sr = new StringRequest(Request.Method.POST,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONArray jsonArray=new JSONArray(response);
                    for(int i=0;i<jsonArray.length();i++){
                        JSONObject object=jsonArray.getJSONObject(i);
                        String resultado=object.getString("resultado");
                        String equipoLocal=object.getString("equipoLocal");
                        String estadio=object.getString("estadio");
                        String fecha=object.getString("fecha");
                        String equipoVisita=object.getString("equipoVisita");
                        String asistencia=object.getString("asistencia");
                        fecha=fecha.replace('.','-');
                        String competencia=object.getString("competencia");
                        String estado=object.getString("estado");

                        ModeloPartido noticias=new ModeloPartido(
                                fecha,
                                resultado,
                                asistencia,
                                estado,
                                competencia,
                                equipoLocal,
                                equipoVisita,
                                estadio
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
                    Toast.makeText(getContext(),"Problema al cargar los Partidos",Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(getContext(),error.getMessage(),Toast.LENGTH_SHORT).show();
                }
            }
        }){
            @Override
            protected Map<String,String> getParams(){
                Map<String,String> params = new HashMap<String, String>();
                UserSessionManager session=new UserSessionManager(getContext());
                Map<String,String> user=session.getUserDetails();
                String NUsario=user.get("usuario");
                params.put("usuario",NUsario);
                params.put("buscar",tipo.get(comboTipo.getSelectedItemPosition()));
                return params;
            }

        };
        queue.add(sr);
    }

    private  void setupRecyclerView(List<ModeloPartido> listNoticias){
        PartidoAdaptador noticiaAdaptador=new PartidoAdaptador(getContext(),this.listNoticias);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        recyclerView.setAdapter(noticiaAdaptador);
    }
}
