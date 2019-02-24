package com.example.carlos.orgaapk.Modelos;

public class ModeloPartido {
    String fecha;
    String resultado;
    String asistencia;
    String estado;
    String competencia;
    String equipoLocal;
    String equipoVisita;
    String estadio;

    public ModeloPartido(String fecha, String resultado, String asistencia, String estado, String competencia, String equipoLocal, String equipoVisita, String estadio) {
        this.fecha = fecha;
        this.resultado = resultado;
        this.asistencia = asistencia;
        this.estado = estado;
        this.competencia = competencia;
        this.equipoLocal = equipoLocal;
        this.equipoVisita = equipoVisita;
        this.estadio = estadio;
    }

    public String getFecha() {
        return fecha;
    }

    public void setFecha(String fecha) {
        this.fecha = fecha;
    }

    public String getResultado() {
        return resultado;
    }

    public void setResultado(String resultado) {
        this.resultado = resultado;
    }

    public String getAsistencia() {
        return asistencia;
    }

    public void setAsistencia(String asistencia) {
        this.asistencia = asistencia;
    }

    public String getEstado() {
        return estado;
    }

    public void setEstado(String estado) {
        this.estado = estado;
    }

    public String getCompetencia() {
        return competencia;
    }

    public void setCompetencia(String competencia) {
        this.competencia = competencia;
    }

    public String getEquipoLocal() {
        return equipoLocal;
    }

    public void setEquipoLocal(String equipoLocal) {
        this.equipoLocal = equipoLocal;
    }

    public String getEquipoVisita() {
        return equipoVisita;
    }

    public void setEquipoVisita(String equipoVisita) {
        this.equipoVisita = equipoVisita;
    }

    public String getEstadio() {
        return estadio;
    }

    public void setEstadio(String estadio) {
        this.estadio = estadio;
    }
}
