<?php
/**
 * Classe de controle do  iniciativa_planejamento_informacoesiniciativa
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON EXPEDITO DORNELAS MORAIS <hemerson.moraiss@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */

include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_informacoesiniciativa.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_informacoesiniciativaHistorico.class.inc';

/**
 * Par3_Controller_Iniciativa_planejamento_informacoesiniciativa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */
class Par3_Controller_Iniciativa_planejamento_informacoesiniciativa
{
    private $model;

    public function __construct()
    {
        $ipifid = $_POST['ipifid'];
        $this->model = new Par3_Model_Iniciativa_planejamento_informacoesiniciativa($ipifid);
        $this->modelHistorico = new Par3_Model_Iniciativa_planejamento_informacoesiniciativaHistorico();
    }



    public function formInformacoesGerais($arrPost)
    {
        global $simec;

        echo '<input type="hidden" name="inpid" value="'.$arrPost["inpid"].'"/>';
        echo '<input type="hidden" name="ipifid" value="'.$this->model->ipifid.'"/>';

        echo '<div class="form-group"><label class="col-md-3 control-label">Código:</label>';
        echo '<div class="col-md-2"><input type="text" disabled="" value="'.$this->model->ipifid.'" class="form-control"></div>';
        echo '<div style="clear:both"></div>';
        echo '</div>';

        echo $simec->input('ipifprofessores', 'Professores Curistas', $this->model->ipifprofessores, array('maxlength' => '255'));
        echo $simec->input('ipiforientadores', 'Orientadores', $this->model->ipiforientadores, array('maxlength' => '255'));
        echo $simec->input('ipifformadores', 'Formadores', $this->model->ipifformadores, array('maxlength' => '255'));
        echo $simec->input('ipiftutores', 'Tutores', $this->model->ipiftutores,array('maxlength' => '255'));
        echo $simec->input('ipifpreparadores', 'Preparadores',$this->model->ipifpreparadores,array('maxlength' => '255'));
        echo $simec->input('ipifsupervisores', 'Supervisores',$this->model->ipifsupervisores,array('maxlength' => '255'));



    }



    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;

//        $this->model = new Par3_Controller_Iniciativa_planejamento_informacoesiniciativa();
        $this->model->popularDadosObjeto();
        $url .= '&ipifid=' . $this->model->ipifid;

        $arrayInformacoes['ipifprofessores']         = trim(($arrPost['ipifprofessores']));
        $arrayInformacoes['ipiforientadores']         = trim(($arrPost['ipiforientadores']));
        $arrayInformacoes['ipifformadores']         = trim(($arrPost['ipifformadores']));
        $arrayInformacoes['ipiftutores']         = trim(($arrPost['ipiftutores']));
        $arrayInformacoes['ipifpreparadores']         = trim(($arrPost['ipifpreparadores']));
        $arrayInformacoes['ipifsupervisores']         = trim(($arrPost['ipifsupervisores']));

        try{
            $this->model->popularDadosObjeto($arrayInformacoes);
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
        $id = $arrPost['ipifid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipifid={$id}";
        $iniciativa_planejamento_informacoesiniciativa = new Par3_Model_Iniciativa_planejamento_informacoesiniciativa($id);

        try{
             $iniciativa_planejamento_informacoesiniciativa->ipifstatus = 'I';
             $iniciativa_planejamento_informacoesiniciativa->ipifdtinativacao = date('Y-m-d H:i:s');
             $iniciativa_planejamento_informacoesiniciativa->ipifusucpfinativacao = $_SESSION['usucpf'];

             $sucesso =   $iniciativa_planejamento_informacoesiniciativa->salvar();
             $iniciativa_planejamento_informacoesiniciativa->commit();

            $this->modelHistorico->gravarHistorico($iniciativa_planejamento_informacoesiniciativa, 'DELETE');

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            return (int)$sucesso;
        }
        return 'erro';
    }


    public function listaInformacoesGerais($inpid, $buff = false)
    {
        $sql = "select  
	   ipin.ipifid ,
 	   ipin.inpid,
	   ipin.ipifprofessores ,
	   ipin.ipiforientadores ,
	   ipin.ipifformadores ,
	   ipin.ipiftutores ,
	   ipin.ipifpreparadores,
	   ipin.ipifsupervisores,
	   ipin.ipifstatus

  FROM par3.iniciativa_planejamento_informacoesiniciativa ipin
  INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = ipin.inpid
  WHERE ipin.inpid = $inpid and ipin.ipifstatus ='A'
  ORDER BY ipin.ipifid";

        $cabecalho = array('Professores Curistas', 'Orientadores', 'Formadores', 'Tutores','Preparadores','Supervisores');
        $esconderColunas = array('ipifid','inpid','ipifstatus');
        
        if($buff){
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        }else{
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        }
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
      //  $listagem->setTamanhoPagina(3);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('remove', 'removerIformacoesGerais');
        $listagem->addAcao('edit', 'editarInformacoesGerais');

        return $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }
}
?>