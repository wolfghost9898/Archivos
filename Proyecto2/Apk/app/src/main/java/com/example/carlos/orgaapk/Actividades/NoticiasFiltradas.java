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
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.example.carlos.orgaapk.Adaptadores.NoticiaAdaptador;
import com.example.carlos.orgaapk.Constantes;
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
 * {@link NoticiasFiltradas.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link NoticiasFiltradas#newInstance} factory method to
 * create an instance of this fragment.
 */
public class NoticiasFiltradas extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    private List<Noticias> listNoticias;
    private RecyclerView recyclerView;

    String url=Constantes.ip+"NoticiasFiltradasA/";


    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public NoticiasFiltradas() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment NoticiasFiltradas.
     */
    // TODO: Rename and change types and number of parameters
    public static NoticiasFiltradas newInstance(String param1, String param2) {
        NoticiasFiltradas fragment = new NoticiasFiltradas();
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
        getActivity().setTitle("Noticias Filtradas");
        View view=inflater.inflate(R.layout.fragment_noticias, container, false);

        listNoticias=new ArrayList<>();
        recyclerView= view.findViewById(R.id.RCNoticias);


        cargarNoticias();



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


    //----------------------------------------------Carga las noticias--------------------------------------------------------
    void  cargarNoticias(){
        RequestQueue queue = Volley.newRequestQueue(getContext());
        StringRequest sr = new StringRequest(Request.Method.POST,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
                    JSONArray jsonArray=new JSONArray(response);
                    for(int i=0;i<jsonArray.length();i++){
                        JSONObject object=jsonArray.getJSONObject(i);
                        String nombre=object.getString("nombre");
                        String apellido=object.getString("apellido");
                        String titulo=object.getString("titulo");
                        String equipo=object.getString("equipo");
                        String cuerpo=object.getString("cuerpo");
                        String fecha=object.getString("fecha");
                        fecha=fecha.replace('.','-');
                        String imagen=object.getString("imagen");

                        imagen=imagen.substring(1,imagen.length());
                        imagen=Constantes.imagenes+imagen;

                        com.example.carlos.orgaapk.Modelos.Noticias noticias=new com.example.carlos.orgaapk.Modelos.Noticias(
                                titulo,
                                cuerpo,
                                fecha,
                                equipo,
                                nombre,
                                apellido,
                                imagen
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
                    Toast.makeText(getContext(),"Problema al cargar las Noticias",Toast.LENGTH_SHORT).show();
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
                return params;
            }

        };
        queue.add(sr);
    }

    private  void setupRecyclerView(List<com.example.carlos.orgaapk.Modelos.Noticias> listNoticias){
        NoticiaAdaptador noticiaAdaptador=new NoticiaAdaptador(getContext(),this.listNoticias);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        recyclerView.setAdapter(noticiaAdaptador);
    }

}
