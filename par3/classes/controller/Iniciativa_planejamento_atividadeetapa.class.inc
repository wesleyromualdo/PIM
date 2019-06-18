<?php

include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_detalheetapa.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_atividadeetapa.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_atividadeetapaHistorico.class.inc';

/**
 * Classe de controle do  iniciativa_planejamento_atividadeetapa
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON EXPEDITO DORNELAS MORAIS <hemerson.moraiss@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */


/**
 * Par3_Controller_Iniciativa_planejamento_atividadeetapa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */
class Par3_Controller_Iniciativa_planejamento_atividadeetapa extends Controle
{
    private $model;

    public function __construct()
    {
        $ipiaid = $_POST['ipiaid'];
        $this->model = new Par3_Model_Iniciativa_planejamento_atividadeetapa($ipiaid);
        $this->modelHistorico = new Par3_Model_Iniciativa_planejamento_atividadeetapaHistorico();
    }

    public function formAtividade($arrPost)
    {

        $mDetalhe = new Par3_Model_Iniciativa_planejamento_detalheetapa();

        $detalheAtividade = $mDetalhe->pegaLinha("SELECT * FROM par3.iniciativa_planejamento_detalheetapa WHERE inpid = {$arrPost["inpid"]}");
        $rsdetalheAtividade = $mDetalhe->pegarSQLSelectCombo($arrPost["inpid"]);


        global $simec;
        echo '<input type="hidden" name="inpid" value="'.$arrPost["inpid"].'"/>';
        echo '<input type="hidden" name="ipiaid" value="'.$this->model->ipiaid.'"/>';
        echo '<div class="form-group"><label class="col-md-3 control-label">Código:</label>';
        echo '<div class="col-md-2"><input type="text" disabled="" value="'.$this->model->ipiaid.'" class="form-control"></div>';
        echo '<div style="clear:both"></div>';
        echo '</div>';
        if($this->model->ipiaid) {

            echo $simec->select('ipidid', 'Atividade', $detalheAtividade['ipidid'], $rsdetalheAtividade,Array('required'));
            echo '<input type="hidden" name="ipidid" value="'.$this->model->ipidid.'"/>';
            echo $simec->input('ipiadescicao', 'Descição Etapa', $this->model->ipiadescicao, array('maxlength' => '255','readonly'=>'true'));

        }else{
            echo $simec->select('ipidid', 'Atividade', $detalheAtividade['ipidid'], $rsdetalheAtividade,Array('required'));
            echo $simec->input('ipiadescicao', 'Descição Etapa', $this->model->ipiadescicao, array('maxlength' => '255','readonly'=>'true'));
        }
        echo $simec->input('ipiadescicaoatividade', 'Descrição Atividade',$this->model->ipiadescicaoatividade, array('maxlength' => '255'));
        echo $simec->input('ipiaquanteventos', 'Quantidade Eventos', $this->model->ipiaquanteventos, array('maxlength' => '6'));
        echo $simec->input('ipiaquantdias', 'Quantidade Dias', $this->model->ipiaquantdias,array('maxlength' => '6'));
        echo $simec->input('ipiaquanthoras', 'Quantidade Horas', $this->model->ipiaquanthoras, array('maxlength' => '6'));
        echo $simec->input('ipiatotal', 'Total', $this->model->ipiatotal,array('type' => 'number','readonly'=>'true'));


        echo '<script>
                                  document.getElementById(\'ipidid\').value ='."'".$this->model->ipidid."'".';
                                
                                $("#ipiaquanthoras,#ipiaquantdias,#ipiaquanteventos").change(function (evt) {
                        
                                evt.preventDefault();
                        
                                var evento = document.getElementById(\'ipiaquanteventos\').value;
                               if(evento == 0 || evento == ""){
                                   evento = 1;
                                }
                                var dias = document.getElementById(\'ipiaquantdias\').value;
                                if(dias == 0 || dias == ""){
                                    dias = 1;
                                }
                                var horas = document.getElementById(\'ipiaquanthoras\').value;
                                if(horas == 0 || horas == ""){
                                    horas = 1;
                                }
                        
                                document.getElementById(\'ipiatotal\').value = (evento * dias * horas);
                                    });
                                                            
                                $(function(){
                                    $(\'#ipiaquanteventos\').bind(\'keydown\',soNums); //
                                    $(\'#ipiaquantdias\').bind(\'keydown\',soNums); //
                                    $(\'#ipiaquanthoras\').bind(\'keydown\',soNums); //
                            
                                });
                                function soNums(e){
                            
                                    //teclas adicionais permitidas (tab,delete,backspace,setas direita e esquerda)
                                    keyCodesPermitidos = new Array(8,9,37,39,46);
                            
                                    //numeros e 0 a 9 do teclado alfanumerico
                                    for(x=48;x<=57;x++){
                                        keyCodesPermitidos.push(x);
                                    }
                            
                                    //numeros e 0 a 9 do teclado numerico
                                    for(x=96;x<=105;x++){
                                        keyCodesPermitidos.push(x);
                                    }
                            
                                    //Pega a tecla digitada
                                    keyCode = e.which;
                            
                                    //Verifica se a tecla digitada é permitida
                                    if ($.inArray(keyCode,keyCodesPermitidos) != -1){
                                        return true;
                                    }
                                    return false;
                                }
                                
                
        $(\'#ipidid\').change(function () {
        var ipidid = $(this).val();
        if (ipidid > 0) {
            montaCombo(ipidid);
        } else {
//            $(\'#crtid\').empty();
//            $(\'#crtid\').trigger("chosen:updated");
        }
    });
     function montaCombo(ipidid) {

        var caminho = window.location.href;
        var action = \'&requisicao=buscaDescItensComposicaoFormacao&identificador=atividadeetapa&iddetalhe=\'+ipidid;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp){    
              //  alert(resp);
                document.getElementById(\'ipiadescicao\').value = resp ;
                return true;
            }
        });
    }
                                
                                
                                
                        </script>';}

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {

        global $url;

        $this->model->popularDadosObjeto();
        $url .= '&ipiaid=' . $this->model->ipiaid;

        $arrayAtividade['ipidatividade']     = $arrPost['ipidatividade'];
        $arrayAtividade['ipiadescicao']     = trim(($arrPost['ipiadescicao']));
        $arrayAtividade['ipiadescicaoatividade']     = trim(($arrPost['ipiadescicaoatividade']));
        $arrayAtividade['ipiaquanteventos']         = $arrPost['ipiaquanteventos'];
        $arrayAtividade['ipiaquantdias']           = $arrPost['ipiaquantdias'];
        $arrayAtividade['ipiaquanthoras']          =$arrPost['ipiaquanthoras'];
        $arrayAtividade['ipiatotal']               = $arrPost['ipiatotal'];

        if($_POST['ipidid'] == null or $_POST['ipidid'] ==""){
            return 'erro';
        }


        try{
            $this->model->popularDadosObjeto($arrayAtividade);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $this->modelHistorico->gravarHistorico($this->model, 'CREATE');
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            return (int)$sucesso;
        }
        return 'erro';
    }



    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['ipiaid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipiaid={$id}";
        $iniciativa_planejamento_atividadeetapa = new Par3_Model_Iniciativa_planejamento_atividadeetapa($id);

        try {

            $iniciativa_planejamento_atividadeetapa->ipiastatus = 'I';
            $iniciativa_planejamento_atividadeetapa->ipiausucpfinativacao = date('Y-m-d H:i:s');
            $iniciativa_planejamento_atividadeetapa->ipiausucpfinativacao = $_SESSION['usucpf'];

            $sucesso =    $iniciativa_planejamento_atividadeetapa->salvar();
            $iniciativa_planejamento_atividadeetapa->commit();

            $this->modelHistorico->gravarHistorico($iniciativa_planejamento_atividadeetapa, 'DELETE');


        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if($sucesso){
            return (int)$sucesso;
        }
        return 'erro';

    }


    public function listarAtividade($inpid, $buff = false)
    {
        $sql = "SELECT    ipa.ipiaid, 
                          ipa.ipidid,
                          ipd.ipidatividade,
                          ipa.ipiadescicao ,
                          ipa.ipiadescicaoatividade ,
                          ipa.ipiaquanteventos,
                          ipa.ipiaquantdias, 
                          ipa.ipiaquanthoras, 
                          ipa.ipiatotal,
                          ipa.ipiastatus
                    FROM par3.iniciativa_planejamento_atividadeetapa ipa
                    INNER join par3.iniciativa_planejamento_detalheetapa ipd on ipd.ipidid =  ipa.ipidid
                    WHERE  ipa.ipiastatus = 'A'  and  ipd.inpid  = $inpid
                    ORDER BY ipa.ipiaid";
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Etapa', 'Descrição Etapa','Descrição Atividade', 'Quantidade de Eventos', 'Quantidade de Dias','Quantidade de Horas','Total');
        $esconderColunas = array('ipiaid','ipidid','ipiastatus');
        
        if($buff){
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        }else{
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        }
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('remove', 'removeratividadeEtapas');
        $listagem->addAcao('edit', 'editarAtividadeEtapas');
      //  $listagem->turnOnOrdenacao();
        return $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function totalCargaHoraria($inpid)
    {

        global $db;
      $result = pg_fetch_array(($db->executar("SELECT SUM(ipa.ipiatotal) as totalcarga 
                    FROM par3.iniciativa_planejamento_atividadeetapa ipa
                    INNER join par3.iniciativa_planejamento_detalheetapa ipd on ipd.ipidid =  ipa.ipidid
                    WHERE  ipa.ipiastatus = 'A'  and ipd.inpid  = $inpid")));
        $total = $result[0];
        if($result[0]){
             return $result[0];
        }


    }
        public function buscaDsc($ipidid){

            global $db;

            $result = pg_fetch_array(($db->executar("SELECT ipiddescricao as descricao FROM par3.iniciativa_planejamento_detalheetapa WHERE ipidid  = $ipidid")));

            return $result[0];

    }
    
}

?>