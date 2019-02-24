from .models import *
from rest_framework import serializers

class UsuarioSerializer(serializers.ModelSerializer):
    class Meta:
        model=Usuario
        fields='__all__'

class NoticiasSerializer(serializers.ModelSerializer):
    class Meta:
        model=Noticias
        fields='__all__'


class PartidoSerializer(serializers.ModelSerializer):
    class Meta:
        model=Partido
        fields='__all__'


class  EquiposSerialize(serializers.ModelSerializer):
    class Meta:
        model=Equipo
        fields='__all__'
