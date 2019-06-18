<?php

if(!isset($_REQUEST['req'])){
    include APPRAIZ.'par3/modulos/sistema/tabelaapoio/geral/configuracoes_par.inc';
    die();
}else{

   include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
    monta_titulo('Exportacao PAR','');

    $justificativa = $_POST['justificativa'];
    $justificativa = str_replace("\'","'", $justificativa);
    ?>
    <form method="post">
        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" style="width: 700px;">
            <tr>
                <td>Tipo:</td>
                <td>
                    <input type="radio" name="tipo" <?=($_POST['tipo'] && $_POST['tipo'] == 'C')? 'checked="checked"' : ''?> value="C">Carregamento &nbsp;
                    <input type="radio" name="tipo" <?=($_POST['tipo'] && $_POST['tipo'] == 'E')? 'checked="checked"' : ''?> value="E">Execução
                </td>
            </tr>
            <tr>
                <td>Colar:</td>
                <td><textarea name="justificativa" cols="100" rows="20"><?=($_POST['justificativa'])? $justificativa:''?></textarea></td>
            </tr>
            <tr>
                <td colspan="2"><input type="submit" /></td>
            </tr>
        </table>
    </form>

 <?php
    if($_POST)
    {
        if($_POST['tipo'] != '' && $_POST['justificativa'] != '' || !in_array($_POST['tipo'], array('E','C')))
        {
            global $db;

            $justificativa = $_POST['justificativa'];
            $justificativa = str_replace("\'","'", $justificativa);

                  if($_POST['tipo'] == 'E'){
                      try{
                         $db->executar($justificativa);
                         $db->commit();
                      }catch (Exception $e){
                         $db->rollback();
                         ver($e->getMessage());
                      }

                  }else{

                      $dados = $db->carregar($justificativa);
                      ver($dados);
                  }

        }else{
            exit('Parametros inválidos');
        }


    }  } ?>