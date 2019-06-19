<?php

use Modules\Seguranca\Entities\PerfilUsuario;
use Modules\Seguranca\Entities\Usuario;

function isActiveRoute($route, $output = 'active')
{
    if (Route::currentRouteName() == $route) {
        return $output;
    }
}

function exibirListasdePerfis($usucpf, $sisid)
{
    $perfisUsuarioOrigem = PerfilUsuario::pegaPerfilPorUsuario($usucpf, $sisid);

    foreach ($perfisUsuarioOrigem as $perfilUsuarioOrigem) {
        if ($perfilUsuarioOrigem['pfldsc'] == 'Super Usuário')
            echo "<span class='label label-success'> {$perfilUsuarioOrigem['pfldsc']}</span>";
        else
            echo "<span class='label label-primary'> {$perfilUsuarioOrigem['pfldsc']}</span>";
    }
}

function exibirListasdeAvisos($usucpf)
{
    if (!empty($usucpf)) {
        $avisosusuario = Usuario::getAvisoUsuarios($usucpf);

        if (is_array($avisosusuario)) {
            $saida = <<<HTML
<table class='table table-bordered table-condensed table-striped'>
                            <tbody>
HTML;

            foreach ($avisosusuario as $valor) {
                $nova = "";
                if ($valor['avbstatus'] == 'A') {
                    $nova = " background-color:#D3D3D3";
                }
                $saida .= <<<HTML
    <tr style='{$nova}'>
        <td> Em <b>{$valor['datahora']}</b> no sistema <b>{$valor['sisabrev']}</b> você recebeu a mensagem: {$valor['avbmsg']}</td>
        <td><a class='confirmarAviso' title='Ir para a referência.' href = '{$valor['avburl']}' target='_top' ><span class='glyphicon glyphicon-screenshot' aria-hidden='true'></span></a</td>
    </tr>
HTML;
            }
            $saida .= <<<HTML
</body>
</table>
HTML;
            echo $saida;
        }
    }
}


function getOptionSistemas($usucpf)
{

    if (!empty($usucpf) || isset($_SESSION['sistemas'][$_SESSION['usucpf']])) {
            $sistemas = Usuario::getSistemasPorCpf($usucpf);
            $_SESSION['sistemas'][$_SESSION['usucpf']] = $sistemas;


        foreach ($sistemas as $sistema) {
            $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null;
            $abreviacaoNomeSistema =  $sistema['sisabrev'];
            echo "<option {$selected}  value='{$sistema['sisid']}'>{$abreviacaoNomeSistema}</option>";
        }
    }

}

function getUnidadeOrcamentaria()
{

    if( true == $_SESSION['superuser'] && $_SESSION['sisdiretorio'] == 'recorcv2'){

        try {

            if(!isset($_SESSION['unoid'])){
                $_SESSION['unoid'] = 4;
            }

            $uo = \Modules\Recorcv2\Entities\UnidadeOrcamentaria::all();
            foreach ($uo as $u) {
                $selected = ($u['unoid'] == $_SESSION['unoid']) ? 'selected="selected"' : null;
                echo "<option {$selected} value='{$u['unoid']}'>{$u['unodsc']}</option>";
            }
        } catch (Exception $e){
            echo"<option value=''>$e</option>";
                echo"<option value=''>Erro ao carregar combo</option>";
//                echo"<option value=''>$e->getMessage();</option>";
        }

    }

}
