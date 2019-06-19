<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/28/2017
 * Time: 3:09 PM
 */

class Par3_Helper_UrlToReturnHelper
{

    private $urlReturn;
    private $linkTextVoltar;
    private $urlComplemento;

    /**
     * Par3_Helper_UrlToReturnHelper constructor.
     * @param $toReturn
     */
    public function __construct(){
        switch ($_REQUEST['toReturn']){
            case 'formListaObraPlanejamento':
                $url = 'par3.php?modulo=principal/planoTrabalho/obraPlanejamento&acao=A&inuid='.$_REQUEST['inuid'].'&toReturn=formListaObraPlanejamento';
                $this->urlReturn = $url;
                $this->linkTextVoltar = 'Voltar para obras';
                $this->urlComplemento = '&toReturn=formListaObraPlanejamento';
                break;
            case 'formListaObra':
                $url = 'par3.php?modulo=principal/listaObras&acao=A&toReturn=formListaObra';
                $this->urlReturn = $url;
                $this->urlComplemento = '&toReturn=formListaObra';
                $this->linkTextVoltar = 'Voltar para lista de obras';
                break;
            default:
                $url = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid='.$_REQUEST['inuid'].'&aba=cadastro&inpid='.$_REQUEST['inpid'];
                $this->urlReturn = $url;
                $this->urlComplemento = '';
                $this->linkTextVoltar = 'Voltar para a iniciativa';
                break;
        }
    }

    public function renderUrlReturn(){
        return $this->urlReturn;
    }

    public function renderLinkVoltarObras(){
       $html = "<a href='{$this->urlReturn}'><i class=\"fa fa-reply-all\" aria-hidden=\"true\"></i>".$this->linkTextVoltar."</a>";
       return $html;
    }

    public function renderComplementoUrl(){
        return $this->urlComplemento;
    }
}