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
import com.example.carlos.orgaapk.Modelos.Noticias;
import com.example.carlos.orgaapk.R;

import org.w3c.dom.Text;

import java.util.List;

public class NoticiaAdaptador  extends RecyclerView.Adapter<NoticiaAdaptador.MyViewHolder> {

    private Context mContext;
    private List<Noticias> mData;
    RequestOptions option;

    public NoticiaAdaptador(Context mContext, List<Noticias> mData) {
        this.mContext = mContext;
        this.mData = mData;
        option=new RequestOptions().centerCrop().placeholder(R.drawable.loading_shape);
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view;
        LayoutInflater inflater=LayoutInflater.from(mContext);
        view=inflater.inflate(R.layout.noticia_item,viewGroup,false);
        return new MyViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder holder, int i) {
        holder.titulo.setText(mData.get(i).getTitulo());
        holder.cuerpo.setText(mData.get(i).getCuerpo());
        holder.nombre.setText("Autor: "+mData.get(i).getNombre()+" "+mData.get(i).getApellido());
        holder.fecha.setText(mData.get(i).getFecha());
        holder.equipo.setText("Equipo: "+mData.get(i).getEquipo());

        Glide.with(mContext).load(mData.get(i).getImagen_url()).apply(option).into(holder.img);
    }

    @Override
    public int getItemCount() {
        return mData.size();
    }

    public static class MyViewHolder extends RecyclerView.ViewHolder{

        TextView titulo;
        TextView nombre;
        TextView cuerpo;
        TextView equipo;
        TextView fecha;
        ImageView img;

        public MyViewHolder(View itemView){
            super(itemView);
            titulo=itemView.findViewById(R.id.NoticiaTitulo);
            nombre=itemView.findViewById(R.id.PublicadoNoticia);
            cuerpo=itemView.findViewById(R.id.descripcion);
            equipo=itemView.findViewById(R.id.EquipoNoticia);
            fecha=itemView.findViewById(R.id.FechaNoticia);
            img=itemView.findViewById(R.id.imagenNoticia);
        }
    }
}
