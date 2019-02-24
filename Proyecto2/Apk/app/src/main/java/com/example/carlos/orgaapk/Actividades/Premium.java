package com.example.carlos.orgaapk.Actividades;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
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
import com.example.carlos.orgaapk.UserSessionManager;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link Premium.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link Premium#newInstance} factory method to
 * create an instance of this fragment.
 */
public class Premium extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";
    String url=Constantes.ip+"cancelarSuscripcionA/";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    Button cancelar;

    private OnFragmentInteractionListener mListener;

    public Premium() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment Premium.
     */
    // TODO: Rename and change types and number of parameters
    public static Premium newInstance(String param1, String param2) {
        Premium fragment = new Premium();
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
        getActivity().setTitle("Suscripcion Premium");
        View view=inflater.inflate(R.layout.fragment_premium, container, false);

        cancelar=view.findViewById(R.id.cancelarSuscripcion);

        cancelar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cancelarSuscripcion();
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


    void cancelarSuscripcion(){
        RequestQueue queue = Volley.newRequestQueue(getContext());
        StringRequest sr = new StringRequest(Request.Method.POST,url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                UserSessionManager session=new UserSessionManager(getContext());
                session.editarMembresia("0");
                Toast.makeText(getContext(),"Membresia Cancelada con exito",Toast.LENGTH_SHORT).show();
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                NetworkResponse networkResponse=error.networkResponse;
                if(networkResponse!=null && networkResponse.statusCode==404){
                    Toast.makeText(getContext(),"Problema al cancelar La suscripcion",Toast.LENGTH_SHORT).show();
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
}
