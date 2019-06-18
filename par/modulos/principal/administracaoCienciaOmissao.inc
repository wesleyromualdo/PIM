<?php
include_once APPRAIZ . "par/classes/controle/Omissaopcente.class.inc";
include_once APPRAIZ . "par/classes/modelo/Omissaopcente.class.inc";
include_once APPRAIZ . "includes/library/simec/Avisos.class.php";

$sistema_bloqueio = $_SESSION['sisid'];

$objOmissao = new Par_Controller_Omissaopcente();
// 147 Obras 2
// 23 Par 2
// 231 PAR 3
$usucpf_bloqueio = $_SESSION['usucpf'];
if ((($sistema_bloqueio == 147) || ($sistema_bloqueio == 23) || ($sistema_bloqueio == 231)) && ($usucpf_bloqueio != '')) {
    
    // inclui o constante
    
    
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
    } elseif ($sistema_bloqueio == 23) {
        
        include_once APPRAIZ . "www/par/_constantes.php";
        $perfil = $objOmissao->pegaArrayPerfil($usucpf_bloqueio);
        
        // Par PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, PAR_PERFIL_PREFEITO
        if (in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_EDUCACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO_BRASIL_PRO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR_PERFIL_SECRETARIO_ESTADUAL_SAUDE, $perfil) || in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil)) {
            $inuid = $_SESSION['par']['inuid'];
            if ($inuid != '') {
                $valida = TRUE;
            }
        }
        // par = PAR_PERFIL_PREFEITO
    } elseif ($sistema_bloqueio == 231) {
        // PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, PAR3_PERFIL_PREFEITO
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

if ($valida) {
    
    $OmissaoNaoAssinada = $objOmissao->verificaExistenciaOmissaoNaoAssinada($inuid);
    
    if ($_REQUEST['requisicaoAjax'] == "declararCiencia") {
        ob_clean();
        
        // @tramitar a ciencia para true
        if($inuid!= '') {
            
            $arrParamsCiencia['usucpf'] = $usucpf_bloqueio;
            $arrParamsCiencia['inuid'] = $inuid;
            
            if (
                    ( ($sistema_bloqueio == 23) && (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)) ) || 
                    ( ($sistema_bloqueio == 147) && (in_array(PFLCOD_GESTOR_UNIDADE, $perfil)) && ( $objOmissao->isPrefeitoPar($usucpf_bloqueio) )) || 
                    ( ($sistema_bloqueio == 231) && (in_array(PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR3_PERFIL_PREFEITO, $perfil)))
                ) {
                $retorno = $objOmissao->insereCienciaOmissao($arrParamsCiencia);
                if ($retorno) {
                    echo "Ciência da Omissão da Prestação de Contas inserida com sucesso!";
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
    
    if ($OmissaoNaoAssinada) {
        
        $objAvisos = new Avisos();
        $listaProcessosOmissos = $objOmissao->retornaListaProcessosComOmissao($inuid);
        
        // Se for prefeito coloca o ciente, se for apenas usuário o fechar e não bloqueia o restante da tela
        $perfil = $objOmissao->pegaArrayPerfil($usucpf_bloqueio);
        $prefeitoSecretario = FALSE;
        
        if ( ($sistema_bloqueio == 23) && (in_array(PAR_PERFIL_PREFEITO, $perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil))) {
        
            $prefeitoSecretario = TRUE;
        }
        
        if ( ($sistema_bloqueio == 147) && (in_array(PFLCOD_GESTOR_UNIDADE, $perfil)) && ( $objOmissao->isPrefeitoPar($usucpf_bloqueio) )) {
            
            $prefeitoSecretario = TRUE; 
        }
        
        if ( ($sistema_bloqueio == 231) && (in_array(PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) || in_array(PAR3_PERFIL_PREFEITO, $perfil))) {
            $prefeitoSecretario = TRUE; 
        }
        
        $dadosUnidade = $objOmissao->getDadosUnidade($inuid);
        
        $arrParams = Array(
            "inuid" => $inuid,
            "arrRegistros" => $listaProcessosOmissos,
            "prefeito_secretario" => $prefeitoSecretario,
            "dadosUnidade"  => $dadosUnidade
        );
        
        if ($sistema_bloqueio == 147 || $sistema_bloqueio == 23) {
            $objAvisos->getAvisoOmissaoPC($arrParams, true);
        } elseif ($sistema_bloqueio == 231) {
            $objAvisos->getAvisoOmissaoPCPAR3($arrParams, true);
            
        }
        
        if ($prefeitoSecretario) {
            $buttons = "{
                        Confirmar: function () {
                            jQuery(\"#dialog_detalhe_processo\").html('');
                            
                            //jQuery(this).dialog(\"close\");
                            declaraCiencia();
                        }
                    }";
        } else {
            $buttons = "{
                        
                    }";
        }
        
        ?>
<script>
            function declaraCiencia() {
            	
            	if ($('input[name="checkbox_ciencia_omissao"]').is(':checked') == false) {
                    alert('É necessário declarar ciência no campo "Declaro ciência da notificação por omissão de prestar contas do(s) Termo(s) listado(s) acima."');
                }
                else {
                	jQuery.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: "&requisicaoAjax=declararCiencia",
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