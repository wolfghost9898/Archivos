# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('Web', '0016_auto_20181028_1123'),
    ]

    operations = [
        migrations.CreateModel(
            name='BitacoraDT',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Fecha_inicio', models.DateTimeField()),
                ('Fecha_fin', models.DateTimeField()),
            ],
            options={
                'db_table': 'Bitacora_DT',
            },
        ),
        migrations.CreateModel(
            name='DT',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('Nombre', models.CharField(max_length=100)),
                ('Fecha_nac', models.DateTimeField()),
                ('Estado', models.CharField(max_length=100)),
                ('Foto', models.ImageField(upload_to=b'', blank=True)),
                ('Equipo', models.ForeignKey(related_name='EquipoDT', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo')),
                ('Pais', models.ForeignKey(related_name='PaisDT', on_delete=django.db.models.deletion.PROTECT, to='Web.Pais')),
            ],
            options={
                'db_table': 'DT',
            },
        ),
        migrations.AddField(
            model_name='bitacoradt',
            name='DT',
            field=models.ForeignKey(related_name='DTBitacora', on_delete=django.db.models.deletion.PROTECT, to='Web.DT'),
        ),
        migrations.AddField(
            model_name='bitacoradt',
            name='Equipo',
            field=models.ForeignKey(related_name='EquipoDTBitacora', on_delete=django.db.models.deletion.PROTECT, to='Web.Equipo'),
        ),
    ]
