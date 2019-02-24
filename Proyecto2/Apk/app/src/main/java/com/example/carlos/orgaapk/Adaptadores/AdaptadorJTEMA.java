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
import com.example.carlos.orgaapk.Modelos.ModeloJTEMA;
import com.example.carlos.orgaapk.R;

import java.util.List;

public class AdaptadorJTEMA extends RecyclerView.Adapter<AdaptadorJTEMA.MyViewHolder> {

    private Context mContext;
    private List<ModeloJTEMA> mData;
    RequestOptions option;

    public AdaptadorJTEMA(Context mContext, List<ModeloJTEMA> mData) {
        this.mContext = mContext;
        this.mData = mData;
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view;
        LayoutInflater inflater=LayoutInflater.from(mContext);
        view=inflater.inflate(R.layout.jtema_item,viewGroup,false);
        return new MyViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder holder, int i) {
        holder.nombre.setText(mData.get(i).getNombre());
        holder.edad.setText("Edad: "+mData.get(i).getEdad());
        if(mData.get(i).getTipo().equals("0")) holder.tipo.setText("Jugador");
        else holder.tipo.setText("Tecnico");


    }

    @Override
    public int getItemCount() {
        return mData.size();
    }

    public static class MyViewHolder extends RecyclerView.ViewHolder{

        TextView edad;
        TextView nombre;
        TextView tipo;
        public MyViewHolder(View itemView){
            super(itemView);
            edad=itemView.findViewById(R.id.edadJTEMA);
            nombre=itemView.findViewById(R.id.NombreJTEMA);
            tipo=itemView.findViewById(R.id.tipoJTEMA);
        }
    }
}