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
import com.example.carlos.orgaapk.Adaptadores.AdaptadorJTE;
import com.example.carlos.orgaapk.Adaptadores.PartidoAdaptador;
import com.example.carlos.orgaapk.Constantes;
import com.example.carlos.orgaapk.Modelos.ModeloJTE;
import com.example.carlos.orgaapk.Modelos.ModeloPartido;
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
 * {@link JuTecEqui.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link JuTecEqui#newInstance} factory method to
 * create an instance of this fragment.
 */
public class JuTecEqui extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private List<ModeloJTE> listNoticias;
    private RecyclerView recyclerView;

    String url=Constantes.ip+"obtenerEquiposA/";
    String url2=Constantes.ip+"JugadorDTA/";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    ArrayList<String> Equipos;
    ArrayList<Integer> idEquipos;
    Spinner comboEquipos;
    Button enviar;

    public JuTecEqui() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment JuTecEqui.
     */
    // TODO: Rename and change types and number of parameters
    public static JuTecEqui newInstance(String param1, String param2) {
        JuTecEqui fragment = new JuTecEqui();
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
        getActivity().setTitle("Jugadores o Tecnico por Equipo");
        Equipos=new ArrayList<>();
        idEquipos=new ArrayList<>();
        View view= inflater.inflate(R.layout.fragment_ju_tec_equi, container, false);
        comboEquipos=view.findViewById(R.id.comboEquipos);
        Equipos.add("Seleccione un equipo");
        idEquipos.add(0);
        cargarEquipos();

        recyclerView= view.findViewById(R.id.RCJTE);

        comboEquipos.setAdapter(new ArrayAdapter<String>(getContext(),android.R.layout.simple_spinner_dropdown_item,Equipos));
        enviar=view.findViewById(R.id.enviarJTE);

        enviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listNoticias=new ArrayList<>();
                obtenerJTE();
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

    void cargarEquipos(){
        RequestQueue queue = Volley.newRequestQueue(getContext());
        StringRequest sr = new StringRequest(Request.Method.GET,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONArray jsonArray=new JSONArray(response);
                    for(int i=0;i<jsonArray.length();i++){
                        JSONObject object=jsonArray.getJSONObject(i);
                        String id=object.getString("id");
                        String nombre=object.getString("Nombre");
                        int idT=Integer.parseInt(id);

                        Equipos.add(nombre);
                        idEquipos.add(idT);


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
                    Toast.makeText(getContext(),"Problema al cargar los Equipos",Toast.LENGTH_SHORT).show();
                }else{
                    Toast.makeText(getContext(),error.getMessage(),Toast.LENGTH_SHORT).show();
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

    void obtenerJTE(){
        RequestQueue queue = Volley.newRequestQueue(getContext());
        StringRequest sr = new StringRequest(Request.Method.POST,url2, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONArray jsonArray=new JSONArray(response);
                    for(int i=0;i<jsonArray.length();i++){
                        JSONObject object=jsonArray.getJSONObject(i);
                        String fechainicio=object.getString("fechainicio");
                        String fechafin=object.getString("fechafin");
                        String nombre=object.getString("nombre");
                        String tipo=object.getString("tipo");

                        ModeloJTE noticias= new ModeloJTE(
                                fechainicio,fechafin,nombre,tipo
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
                params.put("equipo",Equipos.get(comboEquipos.getSelectedItemPosition()));

                return params;
            }

        };
        queue.add(sr);

    }

    private  void setupRecyclerView(List<ModeloJTE> listNoticias){
        AdaptadorJTE noticiaAdaptador=new AdaptadorJTE(getContext(),this.listNoticias);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        recyclerView.setAdapter(noticiaAdaptador);
    }
}


