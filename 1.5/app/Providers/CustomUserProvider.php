<?php

namespace App\Providers;

use App\User;
use Carbon\Carbon;
use Illuminate\Contracts\Auth\Authenticatable;
use Illuminate\Contracts\Auth\UserProvider;

class CustomUserProvider implements UserProvider
{

    /**
     * SimecUserProvider constructor.
     */
    public function __construct()
    {
    }

    /**
     * Retrieve a user by their unique identifier.
     *
     * @param  mixed $identifier
     * @return \Illuminate\Contracts\Auth\Authenticatable|null
     */
    public function retrieveById($identifier)
    {
        $qry = User::where('usucpf','=',$identifier)->where('suscod','=','A');

        if ($qry->count() > 0) {
            $user = $qry->select('*')->first();

            $attributes = [
                'id'       => $user->usucpf,
                'username' => $user->usunome,
                'password' => $user->ususenha,
                'name'     => $user->usunome
            ];

            return $user;
        }
        return null;
    }

    /**
     * Retrieve a user by their unique identifier and "remember me" token.
     *
     * @param  mixed $identifier
     * @param  string $token
     * @return \Illuminate\Contracts\Auth\Authenticatable|null
     */
    public function retrieveByToken($identifier, $token)
    {
        $qry = User::where('usucpf','=',$identifier)->where('suscod','=','A');


        if ($qry->count() > 0) {
            $user = $qry->select('*')->first();

            $attributes = [
                'id'       => $user->usucpf,
                'username' => $user->usunome,
                'password' => $user->ususenha,
                'name'     => $user->usunome
            ];

            return $user;
        }
        return null;
    }

    /**
     * Update the "remember me" token for the given user in storage.
     *
     * @param  \Illuminate\Contracts\Auth\Authenticatable $user
     * @param  string $token
     * @return void
     */
    public function updateRememberToken(Authenticatable $user, $token)
    {
        // TODO: Implement updateRememberToken() method.
        $user->setRememberToken($token);

        $user->save();
    }

    /**
     * Retrieve a user by the given credentials.
     *
     * @param  array $credentials
     * @return \Illuminate\Contracts\Auth\Authenticatable|null
     */
    public function retrieveByCredentials(array $credentials)
    {
        $qry = User::where('usucpf','=',$credentials['cpf'])->where('suscod','=','A');
        if ($qry->count() > 0) {
            $user = $qry->select('*')->first();
            $attributes = [
                'id'       => $user->usucpf,
                'username' => $user->usunome,
                'password' => $user->ususenha,
                'name'     => $user->usunome
            ];

            return $user;
        }
        return null;
    }

    /**
     * Validate a user against the given credentials.
     *
     * @param  \Illuminate\Contracts\Auth\Authenticatable $user
     * @param  array $credentials
     * @return bool
     */
    public function validateCredentials(Authenticatable $user, array $credentials)
    {
        $validarSenha = function($enc_text, $password, $iv_len = 16)
        {
            $enc_text = base64_decode($enc_text);
            $n = strlen($enc_text);
            $i = $iv_len;
            $plain_text = '';
            $iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
            while ($i < $n) {
                $block = substr($enc_text, $i, 16);
                $plain_text .= $block ^ pack('H*', md5($iv));
                $iv = substr($block . $iv, 0, 512) ^ $password;
                $i += 16;
            }
            return preg_replace('/\\x13\\x00*$/', '', $plain_text);
        };

        if(
            $user->usucpf == $credentials['cpf']
            &&
            $credentials['senha'] == $validarSenha($user->getAuthPassword(),'')
        ) {
            $user->usudataultacesso =  Carbon::now();
            $user->usutentativas = 0;
            $user->suscod = 'A';
            $user->timestamps = false;
            $user->save();
            return true;
        }

        /**
         * Grava n￺mero de tentativas
         */
        $tentativas = ($user->usutentativas === 0)?:1;
        $user->usutentativas += $tentativas;
        if($user->usutentativas === 4) {
            $user->suscod = 'B';
        }
        $user->timestamps = false;
        $user->save();
        /**
         * TODO gravar n￺mero de tentativas
         */
        return false;
    }
}