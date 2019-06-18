<?php

/**
 * Centraliza as requisições ajax do módulo.  
 *
 * @author Luciano <lucianoribeiro@mec.gov.br> 
 * @since 12/05/2016 
 */
    
#CARREGA AS FUNÇÕES GERAIS
include_once "config.inc";

if( !empty($_SESSION['usucpforigem']) ){
    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/library/simec/funcoes.inc";
    include_once APPRAIZ . "includes/classes/Controle.class.inc";
    include_once APPRAIZ . "includes/classes/Visao.class.inc";
        
    #CARREGA AS FUNÇÕES DO MÓDULO
    include_once '_constantes.php';
    include_once '_funcoes.php';
    include_once '_componentes.php';
    include_once 'autoload.php';
    
    initAutoload();

    #ABRE CONEXÃO COM O SERVIDOR DE BANCO DE DADOS
    $db = new cls_banco();

    #CAMADA DE CONTROLE "conselho alimentação"
    $ctr_cae = new Par3_Controller_CAE();
    

    #SWITCH CASE - COORDENADOR DA INSTITUIÇÃO
    switch( $_REQUEST['action'] ){
        case 'verificaVicePresidente':
            $ctr_cae->verificaVicePresidente($_REQUEST);
            die();
    }
}