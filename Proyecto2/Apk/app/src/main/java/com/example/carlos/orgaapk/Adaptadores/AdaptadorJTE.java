package com.example.carlos.orgaapk.Adaptadores;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.bumptech.glide.request.RequestOptions;
import com.example.carlos.orgaapk.Modelos.ModeloJTE;
import com.example.carlos.orgaapk.Modelos.ModeloPartido;
import com.example.carlos.orgaapk.R;

import java.util.List;

public class AdaptadorJTE extends RecyclerView.Adapter<AdaptadorJTE.MyViewHolder> {

    private Context mContext;
    private List<ModeloJTE> mData;
    RequestOptions option;

    public AdaptadorJTE(Context mContext, List<ModeloJTE> mData) {
        this.mContext = mContext;
        this.mData = mData;
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view;
        LayoutInflater inflater=LayoutInflater.from(mContext);
        view=inflater.inflate(R.layout.jte_item,viewGroup,false);
        return new MyViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder holder, int i) {
        holder.nombre.setText(mData.get(i).getNombre());
        holder.fechafin.setText("Fecha Fin: "+mData.get(i).getFechafin());
        holder.fechainicio.setText("Fecha Incio: "+mData.get(i).getFechainicio());
        if(mData.get(i).getTipo().equals("0")) holder.tipo.setText("Jugador");
        else holder.tipo.setText("Tecnico");


    }

    @Override
    public int getItemCount() {
        return mData.size();
    }

    public static class MyViewHolder extends RecyclerView.ViewHolder{

        TextView fechainicio;
        TextView fechafin;
        TextView nombre;
        TextView tipo;
        public MyViewHolder(View itemView){
            super(itemView);
            fechainicio=itemView.findViewById(R.id.fechaInicioJTE);
            fechafin=itemView.findViewById(R.id.fechafinJTE);
            nombre=itemView.findViewById(R.id.NombreJTE);
            tipo=itemView.findViewById(R.id.tipoJTE);
        }
    }
}