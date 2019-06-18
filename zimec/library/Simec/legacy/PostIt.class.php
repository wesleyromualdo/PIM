<?php

/**
 * Componente de Post-It, criado para Exibir Avisos de Pendências.
 * include_once APPRAIZ ."includes/library/simec/PostIt.class.php";
$postIt = new PostIt();
$arrCabecalho = array('Ação', 'Número do Termo', 'Processo', 'Descrição do Ítem');
$arrData = array(
    array('Ação1', 'Número do Termo1', 'Processo1', 'Descrição do Ítem1'),
    array('Ação2', 'Número do Termo2', 'Processo2', 'Descrição do Ítem2'),
    array('Ação3', 'Número do Termo3', 'Processo3', 'Descrição do Ítem3'),
    array('Ação4', 'Número do Termo4', 'Processo4', 'Descrição do Ítem4'),
    array('Ação5', 'Número do Termo5', 'Processo5', 'Descrição do Ítem5')
);
$postIt->add('Teste de Utilização de Tabelas', 'Bacon ipsum dolor amet biltong short ribs tenderloin ham, hamburger flank alcatra kevin shank leberkas. Pork loin meatball andouille bresaola drumstick, prosciutto spare ribs strip steak ground round fatback pancetta pork chop short ribs t-bone hamburger. Cow sirloin prosciutto turducken drumstick tail frankfurter capicola.', array('table' => array('coluna' => $arrCabecalho, 'data' => $arrData)));
$postIt->add('Teste do Murilo', 'sadfasdfasdfasdfadfasdfasdf', array('size' => '50', 'cor' => PostIt::SUCCESS, 'table' => array('data' => $arrData, 'coluna' => $arrCabecalho)));
$postIt->add('Teste do Murilo', 'sadfasdfasdfasdfadfasdfasdf', array('size' => '50', 'cor' => PostIt::INFO, 'table' => array('data' => $arrData, 'coluna' => $arrCabecalho)));
$postIt->render();

 *
 * @author Jair Foro <jairforo@gmail.com, jairsantos@mec.gov.br>
 * @since 1.0.0
 */
class PostIt {
    
    const INFO = '#d9edf7';
    const SUCCESS = '#dff0d8';
    const WARNIG = '#fcf8e3';
    const DANGER = '#f2dede';
    
    protected $arrPostIt;
    protected $evento;
    protected $idModal;
    protected $cor;
    
    
    /**
     * Inicia o Objeto PostIt
     * @param type $evento
     * @param type $idModal
     */
    public function __construct($evento = 'ready', $idModal = 'modal_postit') {
        $this->evento = $evento;
        $this->idModal = $idModal;
    }
    
    /*
     * Adicionar Item ao Modal
     */
    public function add($titulo = '', $conteudoHtml = '', $arrParams = array()) {
        $this->arrPostIt[] = array(
            'titulo' => $titulo,
            'conteudoHtml' => $conteudoHtml,
            'arrParams' => $arrParams,
        );
        
        return $this->arrPostIt;
    }
   
   /**
    * Renderiza o Modal e PostIts
    */
   public function render() {
       echo $this->modal();
   }
   
   public function library() {
       $strLibrary = '<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
           <script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>';
       return $strLibrary;
   }
   
   public function modal() {
        $strModal = $this->library();
        $strModal .= $this->evento();
        $strModal .= '<div id="div-postit" style="display: none; font: 11pt Arial">
            <p><strong>Prezados Senhores,</strong> <br />Existem Notificações abaixo que devem ser Tratadas:</p>';
            foreach($this->arrPostIt as $postit) {
                $strModal .= $this->getPosTIt($postit);
            }
        $strModal .= '<div style=\'clear: both\'></div><p>Atenciosamente.<br>Equipe PAR MEC/FNDE.</p></div>';
        
       return $strModal;
   }
   
   public function getPosTIt($arrPostit) {
       # Parametros de Configuração
       $cor = '#fcf8e3';
       $size = '96.4';
       $table = '';
        if (count($arrPostit['arrParams']) > 0) {
            #Cor
            if (key_exists('cor', $arrPostit['arrParams'])) {
                $cor = $arrPostit['arrParams']['cor'] ;
            }
            #Tamanho
            if (key_exists('size', $arrPostit['arrParams'])) {
                $size = $arrPostit['arrParams']['size'] - 2;
            }
            #Tabela
            if (key_exists('table', $arrPostit['arrParams'])) {
                $table = $this->tabela($arrPostit['arrParams']['table']);
            }
        }
        $strPostIt = '
            <style>
                .post-it { 
                    padding:10px; 
                    margin:12px; 
                    -moz-transform: rotate(0deg);
                    -webkit-transform: rotate(0deg);
                    -o-transform: rotate(0deg); 
                    -ms-transform: rotate(0deg); 
                    transform: rotate(0deg); 
                    box-shadow: 0px 4px 6px #333; 
                    -moz-box-shadow: 0px 4px 6px #333; 
                    -webkit-box-shadow: 0px 4px 6px #333; 
                    float: left
                }
            </style>
            <div class="post-it" style="background: '.$cor.'; width: '.$size.'%">
                <div style="width:100%; height: 20px; border-bottom: 1px dashed #fff; margin: 0 0 5px 0">
                    <strong>'.$arrPostit['titulo'].'</strong>
                </div>
                '.$arrPostit['conteudoHtml'].'<div style=\'clear: both; height:20px\'></div>'.$table.'
            </div>';
        return $strPostIt;
   }
   
   public function evento() {
       $strEvento = "			
            <script type=\"text/javascript\">
                jQuery(function(){
                    jQuery(\"#div-postit\").dialog({
                        modal: true,
                        width: '90%',
                        height: 700,
                        title: 'Post-It',
                        open: function(){
                            jQuery('.ui-widget-overlay').bind('click',function(){
                                jQuery('#div-postit').dialog('close');
                            })
                        },
                        buttons: {
                            Fechar: function() {
                                jQuery( this ).dialog( \"close\" );
                            }
                        }
                    });
                });
            </script>";
       return $strEvento;
   }
   
   public function tabela($arrTabela = array()) {
        $strTabela = "<style>
                        table.bordered  {
                            *border-collapse: collapse; /* IE7 and lower */
                            border-spacing: 0;
                            width: 100%;    
                        }

                        .bordered {
                            font-family: 'trebuchet MS', 'Lucida sans', Arial;
                            font-size: 14px;
                            color: #444;
                            border: solid #ccc 1px;
                            -moz-border-radius: 6px;
                            -webkit-border-radius: 6px;
                            border-radius: 6px;
                            -webkit-box-shadow: 0 1px 1px #ccc; 
                            -moz-box-shadow: 0 1px 1px #ccc; 
                            box-shadow: 0 1px 1px #ccc;         
                        }

                        .bordered tr:hover {
                            background: #fbf8e9;
                            -o-transition: all 0.1s ease-in-out;
                            -webkit-transition: all 0.1s ease-in-out;
                            -moz-transition: all 0.1s ease-in-out;
                            -ms-transition: all 0.1s ease-in-out;
                            transition: all 0.1s ease-in-out;     
                        }    

                        .bordered td, .bordered th {
                            border-left: 1px solid #ccc;
                            border-top: 1px solid #ccc;
                            padding: 10px;
                            text-align: left;    
                        }

                        .bordered th {
                            background-color: #dce9f9;
                            background-image: -webkit-gradient(linear, left top, left bottom, from(#ebf3fc), to(#dce9f9));
                            background-image: -webkit-linear-gradient(top, #ebf3fc, #dce9f9);
                            background-image:    -moz-linear-gradient(top, #ebf3fc, #dce9f9);
                            background-image:     -ms-linear-gradient(top, #ebf3fc, #dce9f9);
                            background-image:      -o-linear-gradient(top, #ebf3fc, #dce9f9);
                            background-image:         linear-gradient(top, #ebf3fc, #dce9f9);
                            -webkit-box-shadow: 0 1px 0 rgba(255,255,255,.8) inset; 
                            -moz-box-shadow:0 1px 0 rgba(255,255,255,.8) inset;  
                            box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;        
                            border-top: none;
                            text-shadow: 0 1px 0 rgba(255,255,255,.5); 
                        }

                        .bordered td:first-child, .bordered th:first-child {
                            border-left: none;
                        }

                        .bordered th:first-child {
                            -moz-border-radius: 6px 0 0 0;
                            -webkit-border-radius: 6px 0 0 0;
                            border-radius: 6px 0 0 0;
                        }

                        .bordered th:last-child {
                            -moz-border-radius: 0 6px 0 0;
                            -webkit-border-radius: 0 6px 0 0;
                            border-radius: 0 6px 0 0;
                        }

                        .bordered th:only-child{
                            -moz-border-radius: 6px 6px 0 0;
                            -webkit-border-radius: 6px 6px 0 0;
                            border-radius: 6px 6px 0 0;
                        }

                        .bordered tr:last-child td:first-child {
                            -moz-border-radius: 0 0 0 6px;
                            -webkit-border-radius: 0 0 0 6px;
                            border-radius: 0 0 0 6px;
                        }

                        .bordered tr:last-child td:last-child {
                            -moz-border-radius: 0 0 6px 0;
                            -webkit-border-radius: 0 0 6px 0;
                            border-radius: 0 0 6px 0;
                        }
                      </style>";
        $strTabela .= "<table class=\"bordered\">";
            #Colunas
            if (count($arrTabela['coluna']) > 0) {
                $strTabela .= "<thead><tr>";
                foreach ($arrTabela['coluna'] as $coluna){
                    $strTabela .= "<th>{$coluna}</th>";
                }
                $strTabela .= "</tr></thead>";
            }
            if (count($arrTabela['data']) > 0) {
                $strTabela .= "<tbody>";
                foreach ($arrTabela['data'] as $chave => $valor){
                    $strTabela .= "<tr>";
                    foreach ($valor as $valorCampo) {
                       $strTabela .= "<td>{$valorCampo}</td>"; 
                    }
                    $strTabela .= "</tr>";
                }
                $strTabela .= "</tbody>";
            }
        $strTabela .= "</table>";
        return $strTabela;
   }
}
