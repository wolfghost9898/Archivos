package com.example.carlos.orgaapk.Adaptadores;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.RequestOptions;
import com.example.carlos.orgaapk.Modelos.ModeloPartido;
import com.example.carlos.orgaapk.Modelos.Noticias;
import com.example.carlos.orgaapk.R;

import java.util.List;

public class PartidoAdaptador extends RecyclerView.Adapter<PartidoAdaptador.MyViewHolder> {

    private Context mContext;
    private List<ModeloPartido> mData;
    RequestOptions option;

    public PartidoAdaptador(Context mContext, List<ModeloPartido> mData) {
        this.mContext = mContext;
        this.mData = mData;
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view;
        LayoutInflater inflater=LayoutInflater.from(mContext);
        view=inflater.inflate(R.layout.partido_item,viewGroup,false);
        return new MyViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull PartidoAdaptador.MyViewHolder holder, int i) {
        holder.titulo.setText(mData.get(i).getEquipoLocal()+"-"+mData.get(i).getEquipoVisita());
        holder.resultado.setText("Resultado: "+mData.get(i).getResultado());
        holder.asistencia.setText("Asistencia: "+mData.get(i).getAsistencia());
        holder.fecha.setText(mData.get(i).getFecha());
        holder.estado.setText("Estado: "+mData.get(i).getEstado());
        holder.competencia.setText("Estado: "+mData.get(i).getCompetencia());
        holder.estadio.setText("Estado: "+mData.get(i).getEstadio());

    }

    @Override
    public int getItemCount() {
        return mData.size();
    }

    public static class MyViewHolder extends RecyclerView.ViewHolder{

        TextView fecha;
        TextView resultado;
        TextView asistencia;
        TextView estado;
        TextView competencia;
        TextView titulo;
        TextView estadio;

        public MyViewHolder(View itemView){
            super(itemView);
            fecha=itemView.findViewById(R.id.fechaEquipo);
            resultado=itemView.findViewById(R.id.resultadoPartido);
            asistencia=itemView.findViewById(R.id.asistenciaPartido);
            competencia=itemView.findViewById(R.id.competenciaPartido);
            estado=itemView.findViewById(R.id.estadoPartido);
            titulo=itemView.findViewById(R.id.PartidoVS);
            estadio=itemView.findViewById(R.id.estadioPartido);
        }
    }
}