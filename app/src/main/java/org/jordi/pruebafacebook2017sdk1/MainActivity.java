package org.jordi.pruebafacebook2017sdk1;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.Profile;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.login.widget.LoginButton;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    private String tag = "ImgProcesadoNDK";
    private Bitmap bitmapOriginal = null;
    private Bitmap bitmapGrises = null;
    private Bitmap bitmapSepia = null;
    private Bitmap bitmapMarco1 = null;
    private Bitmap bitmapMarco2 = null;
    private ImageView ivDisplay = null;


    LoginButton loginButtonOficial;
    private CallbackManager elCallbackManagerDeFacebook;
    private final Activity THIS = this;

    static final int REQUEST_IMAGE_CAPTURE = 1;

    static {
        System.loadLibrary("imgprocesadondk");
    }

    public native void convertirGrises(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void convertirSepia(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void crearMarco1(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void crearMarco2(Bitmap bitmapIn, Bitmap bitmapOut);

    public static boolean hayPixel(int x, int y) {
        return x % 10 == y % 10;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.i(tag, "Imagen antes de modificar");
        ivDisplay = (ImageView) findViewById(R.id.ivDisplay);

        loginButtonOficial = (LoginButton) findViewById(R.id.login_button);
        loginButtonOficial.setPublishPermissions("publish_actions");

        this.elCallbackManagerDeFacebook = CallbackManager.Factory.create();

        LoginManager.getInstance().registerCallback(this.elCallbackManagerDeFacebook,
                new FacebookCallback<LoginResult>() {
                    @Override
                    public void onSuccess(LoginResult loginResult) {
                        Toast.makeText(THIS, "Login onSuccess()", Toast.LENGTH_LONG).show();
                        actualizarVentanita();
                    }

                    @Override
                    public void onCancel() {
                        Toast.makeText(THIS, "Login onCancel()", Toast.LENGTH_LONG).show();
                        actualizarVentanita();
                    }

                    @Override
                    public void onError(FacebookException exception) {
                        Toast.makeText(THIS, "Login onError(): " + exception.getMessage(),
                                Toast.LENGTH_LONG).show();
                        actualizarVentanita();
                    }
                });

        this.actualizarVentanita();

    }



    public void ResetImagen() {
        Log.i(tag, "Resetear Imagen");
        ivDisplay.setImageBitmap(bitmapOriginal);
    }

    public void ConvertirGrises() {
        Log.i(tag, "Conversion a escala de grises");
        bitmapGrises = Bitmap.createBitmap(bitmapOriginal.getWidth(), bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
        convertirGrises(bitmapOriginal, bitmapGrises);
        ivDisplay.setImageBitmap(bitmapGrises);
    }

    public void ConvertirSepia() {
        Log.i(tag, "Conversion a sepia");
        bitmapSepia = Bitmap.createBitmap(bitmapOriginal.getWidth(), bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
        convertirSepia(bitmapOriginal, bitmapSepia);
        ivDisplay.setImageBitmap(bitmapSepia);
    }

    public void CrearMarco1() {
        Log.i(tag, "Creando Marco 1");
        bitmapMarco1 = Bitmap.createBitmap(bitmapOriginal.getWidth(), bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
        crearMarco1(bitmapOriginal, bitmapMarco1);
        ivDisplay.setImageBitmap(bitmapMarco1);
    }

    public void CrearMarco2() {
        Log.i(tag, "Creando Marco 2");
        bitmapMarco2 = Bitmap.createBitmap(bitmapOriginal.getWidth(), bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
        crearMarco2(bitmapOriginal, bitmapMarco2);
        ivDisplay.setImageBitmap(bitmapMarco2);
    }

    public void onSacarFoto(View v) {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            startActivityForResult(takePictureIntent, REQUEST_IMAGE_CAPTURE);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
            Bundle extras = data.getExtras();
            Bitmap imageBitmap = (Bitmap) extras.get("data");
            bitmapOriginal = imageBitmap;
            if (bitmapOriginal != null)
                ivDisplay.setImageBitmap(bitmapOriginal);
        }

        this.elCallbackManagerDeFacebook.onActivityResult(requestCode, resultCode, data);
    }

    private void actualizarVentanita()
    {
        AccessToken accessToken = this.obtenerAccessToken();

        if (accessToken== null)
        {

        }
//
// sí hay sesión
//
        Log.d("cuandrav.actualizarVent", "hay sesion habilito");
        //this.botonHacerLogin.setEnabled(false);
//        this.botonLogOut.setEnabled(true);
//        this.textoConElMensaje.setEnabled(true);
//        this.botonCompartir.setEnabled(true);
//        this.botonEnviarFoto.setEnabled(true);
//
// averiguo los datos básicos del usuario acreditado
//
        Profile profile = Profile.getCurrentProfile();
        if (profile != null) {
//            this.textoConElMensaje.setText(profile.getName());
        }
    }

    private boolean sePuedePublicar() {
        if (!this.hayRed()) {
            Toast.makeText(this, "¿no hay red? No puedo publicar", Toast.LENGTH_LONG).show();
            return false;
        }

        if (! this.tengoPermisoParaPublicar()) {
            Toast.makeText(this, "¿no tengo permisos para publicar? Los pido.", Toast.LENGTH_LONG).show();
            LoginManager.getInstance().logInWithPublishPermissions(this, Arrays.asList("publish_actions"));
            return false;
        }
        return true;
    }

    private boolean tengoPermisoParaPublicar() {
        AccessToken accessToken = this.obtenerAccessToken();
        return accessToken != null && accessToken.getPermissions().contains("publish_actions");
    }


    private boolean hayRed() {
        ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetworkInfo = connectivityManager
                .getActiveNetworkInfo();
        return activeNetworkInfo != null && activeNetworkInfo.isConnected();
    }


    private AccessToken obtenerAccessToken() {
        return AccessToken.getCurrentAccessToken();
    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.filtro_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.original) {
            ResetImagen();
        }

        if (id == R.id.grises) {
            ConvertirGrises();
        }

        if (id == R.id.sepia) {
            ConvertirSepia();
        }

        if (id == R.id.marco1) {
            CrearMarco1();
        }

        if (id == R.id.marco2) {
            CrearMarco2();
        }

        return super.onOptionsItemSelected(item);
    }
}
