<?php
include_once APPRAIZ . "par/classes/controle/Omissaopcente.class.inc";
include_once APPRAIZ . "par/classes/modelo/Omissaopcente.class.inc";
include_once APPRAIZ . "par/classes/controle/Omissaopccacs.class.inc";
include_once APPRAIZ . "par/classes/modelo/Omissaopccacs.class.inc";
include_once APPRAIZ . "includes/library/simec/Avisos.class.php";

$sistema_bloqueio = $_SESSION['sisid'];

$objOmissao     = new Par_Controller_Omissaopcente();
$objOmissaoCACS = new Par_Controller_Omissaopccacs();
// 147 Obras 2
// 23 Par 2
// 231 PAR 3
$valida = FALSE;
$usucpf_bloqueio = $_SESSION['usucpf'];
// Caso for Obras, PAR ou PAR 3 
if ((($sistema_bloqueio == 147) || ($sistema_bloqueio == 23) || ($sistema_bloqueio == 231)) && ($usucpf_bloqueio != '')) {
    
    // inclui o constante de acordo com o sistema que está acessando
    if ($sistema_bloqueio == 147) {
        
        include_once APPRAIZ . "www/obras2/_constantes.php";
        $perfil = $objOmissao->pegaArrayPerfil($usucpf_bloqueio);
        
        if (in_array(PFLCOD_GESTOR_UNIDADE, $perfil) || in_array(PFLCOD_SUPERVISOR_UNIDADE, $perfil)) {
            
            $inuid = $objOmissao->getInuidPeloObras($usucpf_bloqueio);
            if ($inuid != '') {
                $valida = TRUE;
            }
            // oBRAS = PFLCOD_GESTOR_UNIDADE
        }
    } else if ($sistema_bloqueio == 23) {
        
        include_once APPRAIZ . "www/par/_constantes.php";
        $perfil = $objOmissao->pegaArrayPerfil($usucpf_bloqueio);
        
        if (in_array(PAR_PERFIL_PRESIDENTE, $perfil) || in_array(PAR_PERFIL_CONSELHEIRO , $perfil) || in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE, $perfil) || in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil)) {
            
            $inuid = $_SESSION['par']['inuid'];
            if ($inuid != '') {
                $valida = TRUE;
            }
        }
        // par = PAR_PERFIL_PREFEITO
    } elseif ($sistema_bloqueio == 231) { 
        include_once APPRAIZ . "www/par3/_constantes.php";
        $perfil = $objOmissao->pegaArrayPerfil($usucpf_bloqueio);
        
        if (in_array(PAR3_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR3_PERFIL_DIRIGENTE_MUNICIPAL, $perfil) || in_array(PAR3_PERFIL_PREFEITO, $perfil) || in_array(PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR3_PERFIL_EQUIPE_ESTADUAL, $perfil)) {
            
            $inuid = $objOmissao->getInuidPeloPar3($usucpf_bloqueio);
            if ($inuid != '') {
                $valida = TRUE;
            }
        }
    }
} else {
    $valida = FALSE;
}


// DECLARA CIENCIA identificando o tipo
if ($_REQUEST['requisicaoAjax'] == "declararCienciaCACS") {
    ob_clean();
    $ropid = ($_REQUEST['ropid']) ? $_REQUEST['ropid'] : '';

    // @tramitar a ciencia para true
    if($inuid!= '' && $ropid != '') {

        $arrParamsCiencia['usucpf'] = $usucpf_bloqueio;
        $arrParamsCiencia['inuid'] = $inuid;
        $arrParamsCiencia['ropid'] = $ropid;

        if (
            ( ($sistema_bloqueio == 23) && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_PRESIDENTE, $perfil)  ) ) ||
            ( ($sistema_bloqueio == 147) && (in_array(PFLCOD_GESTOR_UNIDADE, $perfil)) && ( $objOmissao->isPrefeitoPar($usucpf_bloqueio) )) ||
            ( ($sistema_bloqueio == 231) && (in_array(PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR3_PERFIL_PREFEITO, $perfil)))
        ) {
            // Criar nova função, identificando qual dos dois está assinando
            $retorno = $objOmissaoCACS->insereCienciaOmissao($arrParamsCiencia);
            if ($retorno) {
                echo "Ciência da Omissão do Controle Social inserida com sucesso!";
                exit();
            } else {
                echo "Erro ao declarar a ciência da Omissão da Prestação de Contas 1 ";
                exit();
            }
        } else {
            echo "Erro ao declarar a ciência da Omissão da Prestação de Contas 2 ";
            exit();
        }
    } else {
        echo "Erro ao declarar a ciência da Omissão da Prestação de Contas 3";
        exit();
    }

}

if ($valida) {
    
    // Gestor Unidade
    $OmissaoNaoAssinadaGestor = $objOmissaoCACS->verificaExistenciaOmissaoNaoAssinada($inuid, ROPID_GESTOR_UNIDADE );
    // Presidente CACS
    $OmissaoNaoAssinadaPresidente = $objOmissaoCACS->verificaExistenciaOmissaoNaoAssinada($inuid, ROPID_PRESIDENTE );
    
    // SE FOR GESTOR OU EQUIPE:
    if ( in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE, $perfil) || in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) ||  (in_array(PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR3_PERFIL_PREFEITO, $perfil)) || (in_array(PFLCOD_GESTOR_UNIDADE, $perfil)) || ( $objOmissao->isPrefeitoPar($usucpf_bloqueio) ) || in_array(PFLCOD_SUPERVISOR_UNIDADE, $perfil)  ) {
        $ropid              = ROPID_GESTOR_UNIDADE;
    
    }// sE FOR PRESIDENTE OU MEMBRO
    if(in_array(PAR_PERFIL_PRESIDENTE, $perfil) || in_array(PAR_PERFIL_CONSELHEIRO , $perfil) ) {
        $ropid              = ROPID_PRESIDENTE;
    }
    
    //Caso exista do prefeito ou do cacs entra aqui
    if ( ($OmissaoNaoAssinadaGestor && ($ropid == ROPID_GESTOR_UNIDADE)) || ($OmissaoNaoAssinadaPresidente && ($ropid == ROPID_PRESIDENTE)) ) {
        
        $objAvisos = new Avisos();
        // Retorna a lista dos processos com omissão do CACS  
        $listaProcessosOmissos = $objOmissaoCACS->retornaListaProcessosComOmissao($inuid);
        
        // Se for prefeito coloca o ciente, se for apenas usuário o fechar e não bloqueia o restante da tela
        $perfil = $objOmissao->pegaArrayPerfil($usucpf_bloqueio);
        $prefeitoSecretario = FALSE;
        $presidente         = FALSE;
        
         
        
        // SE for PAR e  cacs
        if ( ($sistema_bloqueio == 23) && (in_array(PAR_PERFIL_PRESIDENTE, $perfil)) ) {
            $presidente         = TRUE;
        }
        
        // SE for OBras 2  prefeito secretario
        // OU for PAR 3  prefeito secretario
        // OU for PAR e  prefeito secretario
        if (( ($sistema_bloqueio == 147) && (in_array(PFLCOD_GESTOR_UNIDADE, $perfil)) && ( $objOmissao->isPrefeitoPar($usucpf_bloqueio) )) 
            || ( ($sistema_bloqueio == 231) && (in_array(PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR3_PERFIL_PREFEITO, $perfil)))
            || ( ($sistema_bloqueio == 23) && (in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil)))) {
            $prefeitoSecretario = TRUE;
        }
        // pego os dados da unidade
        $dadosUnidade = $objOmissao->getDadosUnidade($inuid);
        // Monto parâmetros do usuário que está acessando
        $arrParamsGestor = Array(
            "inuid" => $inuid,
            "arrRegistros" => $listaProcessosOmissos,
            "responsavel" => $prefeitoSecretario,
            "dadosUnidade"  => $dadosUnidade
        );
        
        $arrParamsPresidente = Array(
            "inuid" => $inuid,
            "arrRegistros" => $listaProcessosOmissos,
            "responsavel" => $presidente,
            "dadosUnidade"  => $dadosUnidade
        );
        
        $arrParams = ($ropid == ROPID_GESTOR_UNIDADE) ? $arrParamsGestor : $arrParamsPresidente;
        
        //
        if ($sistema_bloqueio == 147 || $sistema_bloqueio == 23) {
            $objAvisos->getAvisoOmissaoPCCACS($arrParams, $ropid);
        } elseif ($sistema_bloqueio == 231) {
            $objAvisos->getAvisoOmissaoPCPAR3CACS($arrParams, $ropid);
        }
        
        if ($prefeitoSecretario || $presidente ) {
            $buttons = "{
                        Confirmar: function () {
                            jQuery(\"#dialog_detalhe_processo\").html('');
                            
                            //jQuery(this).dialog(\"close\");
                            declaraCiencia({$ropid});
                        }
                    }";
        } else {
            $buttons = "{
                        
                    }";
        }
        
        ?>
<script>
            function declaraCiencia( ropid ) {
                
            	if ($('input[name="checkbox_ciencia_omissao"]').is(':checked') == false) {
                    alert('É necessário declarar ciência no campo "Declaro ciência da notificação por omissão do Controle Social do(s) Termo(s) listado(s) acima."');
                }
                else {
                	jQuery.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: "&requisicaoAjax=declararCienciaCACS&ropid="+ropid,
                        async: false,
                        success: function (msg) {
                            alert(msg);
                            window.location.href = window.location.href;
                        }
                    });
                }
                
            }
       <?php
        
        if ($sistema_bloqueio == 231) {
            
            echo "
            
                function mostraAvisosSU() {
                    jQuery(\"#modal_iniciativaDesc\").modal({backdrop: 'static', keyboard: false});
    
                }
            </script>";
        } else {
            
            echo "
            function mostraAvisosSU() {
            
            jQuery(function () {
            jQuery(\"#dialog_omissao_ente\").dialog({
            modal: true,
            width: '90%',
            height: 700,
            open: function () {
            
            $(\".ui-dialog-titlebar-close\").hide()
            },
            buttons: {$buttons}
            });
            
            });
            }
            </script>";
            
            
        }
        if($sistema_bloqueio != 231){
        ?>

        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"
	src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css"
	href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css" />
<?php
    }
        echo '<br>';
        if ($prefeitoSecretario) {
            die("<script>jQuery('#aguarde').hide();mostraAvisosSU();</script>");
        } else {
            die( "<script>jQuery('#aguarde').hide();mostraAvisosSU();</script>");
        }
        //
    }
}