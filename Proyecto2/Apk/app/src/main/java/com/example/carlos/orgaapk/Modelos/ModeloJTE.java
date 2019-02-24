package com.example.carlos.orgaapk.Modelos;

public class ModeloJTE {
    String fechainicio;
    String fechafin;
    String nombre;
    String tipo;

    public ModeloJTE(String fechainicio, String fechafin, String nombre, String tipo) {
        this.fechainicio = fechainicio;
        this.fechafin = fechafin;
        this.nombre = nombre;
        this.tipo = tipo;
    }

    public String getFechainicio() {
        return fechainicio;
    }

    public void setFechainicio(String fechainicio) {
        this.fechainicio = fechainicio;
    }

    public String getFechafin() {
        return fechafin;
    }

    public void setFechafin(String fechafin) {
        this.fechafin = fechafin;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }
}
