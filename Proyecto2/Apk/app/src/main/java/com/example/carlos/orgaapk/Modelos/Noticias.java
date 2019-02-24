package com.example.carlos.orgaapk.Modelos;

public class Noticias {
    String titulo;
    String cuerpo;
    String fecha;
    String equipo;
    String nombre;
    String apellido;
    String imagen_url;

    public Noticias(String titulo, String cuerpo, String fecha, String equipo, String nombre, String apellido, String imagen_url) {
        this.titulo = titulo;
        this.cuerpo = cuerpo;
        this.fecha = fecha;
        this.equipo = equipo;
        this.nombre = nombre;
        this.apellido = apellido;
        this.imagen_url = imagen_url;
    }

    public String getTitulo() {
        return titulo;
    }

    public void setTitulo(String titulo) {
        this.titulo = titulo;
    }

    public String getCuerpo() {
        return cuerpo;
    }

    public void setCuerpo(String cuerpo) {
        this.cuerpo = cuerpo;
    }

    public String getFecha() {
        return fecha;
    }

    public void setFecha(String fecha) {
        this.fecha = fecha;
    }

    public String getEquipo() {
        return equipo;
    }

    public void setEquipo(String equipo) {
        this.equipo = equipo;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public String getApellido() {
        return apellido;
    }

    public void setApellido(String apellido) {
        this.apellido = apellido;
    }

    public String getImagen_url() {
        return imagen_url;
    }

    public void setImagen_url(String imagen_url) {
        this.imagen_url = imagen_url;
    }
}
