package com.example.carlos.orgaapk;

import android.net.Uri;
import  android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import com.example.carlos.orgaapk.Actividades.Free;
import com.example.carlos.orgaapk.Actividades.JuTecEqui;
import com.example.carlos.orgaapk.Actividades.JugadorTecnicoEMa;
import com.example.carlos.orgaapk.Actividades.JugadorTecnicoEMe;
import com.example.carlos.orgaapk.Actividades.Noticias;
import com.example.carlos.orgaapk.Actividades.NoticiasFiltradas;
import com.example.carlos.orgaapk.Actividades.Partidos;
import com.example.carlos.orgaapk.Actividades.Premium;

import java.util.HashMap;
import java.util.Map;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener ,ManualFragment.OnFragmentInteractionListener,AutoFragment.OnFragmentInteractionListener{
UserSessionManager session;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);




        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        Fragment fragment=new ManualFragment();
        getSupportFragmentManager().beginTransaction().add(R.id.content_main,fragment).commit();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        session=new UserSessionManager(getApplicationContext());

        if(session.checkLogin()) finish();

        HashMap<String,String> user=session.getUserDetails();
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement


        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();
        Fragment mifragment=null;
        boolean fragmentoselect=false;
        Map<String,String> data=session.getUserDetails();
        String membresia=data.get("membresia");
           if(id== R.id.logout){
              session.LogoutUser();

          } else if(id==R.id.noticias){
            mifragment=new Noticias();
            fragmentoselect=true;
          }else if(id==R.id.noticiasFiltradas){
              if(membresia.equals("1")){
                mifragment=new NoticiasFiltradas();
                fragmentoselect=true;
              }else
                  Toast.makeText(getApplicationContext(),"Necesitas ser Premiun",Toast.LENGTH_SHORT).show();
          }else if(id==R.id.membresia){

               if(membresia.equals("1")){
                   mifragment=new Premium();
                   fragmentoselect=true;
               }else{
                   mifragment=new Free();
                   fragmentoselect=true;
               }

           }else if(id==R.id.partidos){
                mifragment=new Partidos();
                fragmentoselect=true;
           }else if(id==R.id.JTE){
                if(membresia.equals("1")){
                    mifragment=new JuTecEqui();
                    fragmentoselect=true;
                }else
                    Toast.makeText(getApplicationContext(),"Necesitas ser Premium",Toast.LENGTH_SHORT);
           }else if(id==R.id.JTEMa){
               if(membresia.equals("1")){
                   mifragment=new JugadorTecnicoEMa();
                   fragmentoselect=true;
               }else
                   Toast.makeText(getApplicationContext(),"Necesitas ser Premium",Toast.LENGTH_SHORT);
           }else if(id==R.id.JTEMe){
               if(membresia.equals("1")){
                   mifragment=new JugadorTecnicoEMe();
                   fragmentoselect=true;
               }else
                   Toast.makeText(getApplicationContext(),"Necesitas ser Premium",Toast.LENGTH_SHORT);
           }

        if (fragmentoselect==true){
getSupportFragmentManager().beginTransaction().replace(R.id.content_main,mifragment).commit();

        }


        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    public void onFragmentInteraction(Uri uri) {

    }




}
