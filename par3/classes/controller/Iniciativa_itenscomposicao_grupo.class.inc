<?php
/**
 * Classe de controle do  iniciativa_itenscomposicao_grupo
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON EXPEDITO DORNELAS MORAIS <hemerson.moraiss@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-07-2017
 * @link     no link
 */

include_once APPRAIZ . 'par3/classes/model/Iniciativa_itenscomposicao_grupoHistorico.class.inc';

/**
 * Par3_Controller_Iniciativa_itenscomposicao_grupo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-07-2017
 * @link     no link
 */
class Par3_Controller_Iniciativa_itenscomposicao_grupo
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Iniciativa_itenscomposicao_grupo($_GET['iigid']);
        $this->modelHistorico = new Par3_Model_Iniciativa_itenscomposicao_grupoHistorico();
    }

    public function salvarTiposObra($arrPost,$iniid)
    {
        $this->model->salvarTiposObra($arrPost, $iniid);
    }
    public function buscaCategoria($iniid)
    {
        return $this->model->buscaCategoria($iniid);
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;

        $iniid = $arrPost['iniid'];

        $validacao = $this->verificaDados($arrPost,$iniid);


        if ($validacao) {

            $arrayIniciativa['igrid'] = $arrPost['igrid'];
            $arrayIniciativa['itcid'] = $arrPost['itcid'];
            $arrayIniciativa['iniid'] = $iniid;

            try {
                $this->model->popularDadosObjeto($arrayIniciativa);
                $id = $this->model->salvar();
                $this->model->commit();
                $this->modelHistorico->gravarHistorico($this->model, 'CREATE');
                $this->modelHistorico->commit();

                if($arrPost['proinf'] == 't' && $arrPost['igrid']){
                    $this->model->salvarItensKitProinfancia($arrayIniciativa);
                }

                //  $id = 1;
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }
            if ($id) {
                return $id;
            }
            return 'erro';

        }
  return 'erro';

    }

    public function excluirTipoObra($arrPost)
    {
        $this->model->excluirTipoObra($arrPost);

        return true;
    }

    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_POST['iigid'];

        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&iigid={$id}";
        $iniciativa_itenscomposicao_grupo = new Par3_Model_Iniciativa_itenscomposicao_grupo($id);

       // var_dump($iniciativa_itenscomposicao_grupo);exit;
        try {
            $iniciativa_itenscomposicao_grupo->iigsituacao = 'I';
            $iniciativa_itenscomposicao_grupo->iigdtinativacao = date('Y-m-d H:i:s');
            $iniciativa_itenscomposicao_grupo->iigcpfinativacao = $_SESSION['usucpf'];


            $iniciativa_itenscomposicao_grupo->salvar();
            $iniciativa_itenscomposicao_grupo->commit();


            $this->modelHistorico->gravarHistorico($iniciativa_itenscomposicao_grupo, 'DELETE');

            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

// função que verifica se já o item o grupo cadastrado para a ainiciativa
    public function verificaDados($arrPost,$iniid)
    {

        if ($arrPost['itcid'] != "" || $arrPost['iniid'] != "") {

            $where = $arrPost['itcid'] ? " AND itcid = '{$arrPost['itcid']}'" : '';
            $where .= $arrPost['iniid'] ? " AND iniid  ='{$iniid}'" : '';
            $where .= $arrPost['igrid'] ? " AND igrid  ='{$arrPost['igrid']}'" : '';
            $where .= " AND iigsituacao  = 'A'";

        }
        $sql = "SELECT   *  FROM  par3.iniciativa_itenscomposicao_grupo iig where 1 = 1 $where";

        $result = (pg_num_rows(pg_query($sql)));


        if ($result != 0) {
            $result = false;
            return $result;
        }
        $result = true;
        return $result;

    }

    public function listarItems($iniid)
    {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);

        $mdIniciativa = new Par3_Model_Iniciativa($iniid);

        if($mdIniciativa->intoid != '1'){
            $sql = "SELECT iig.iigid, 
                           itc.itcid,
                           itc.itcdsc,
                           itc.itcespecificacao,
                           ctg.ctddsc,
                           und.unidsc,
                           itc.itcsituacao as situacao
                    FROM par3.iniciativa_itenscomposicao_grupo iig
                    INNER JOIN par3.itenscomposicao     itc ON itc.itcid = iig.itcid
                    INNER JOIN par3.categoriadespesa    ctg ON ctg.ctdid = itc.ctdid
                    INNER JOIN par3.unidade_medida      und ON und.uniid = itc.uniid
                    WHERE iig.iigsituacao = 'A' AND iig.iniid = $iniid AND igrid IS NULL";
            $cabecalho = array('Código', 'Item', 'Especificação Técnica', 'Unidade de Medida', 'Categoria de Despesa', 'Situação');
            $listagem->addAcao('remove', 'removerItem');
        }else{
            $sql = "SELECT
                    	otp.otpid,
                        array_to_string(
                            array(
                                SELECT distinct etadsc
                                FROM par3.ensino_etapa eta
                                INNER JOIN par3.obra_tipo_ensino_etapa ote ON ote.etaid = eta.etaid
                                WHERE ote.otpid = otp.otpid), ', ', 'N/A') as etapas,
                    	oct.octdsc,
                    	otp.otpid||' - '||otp.otpdsc
                    FROM
                    	par3.obra_tipo otp
                    INNER JOIN par3.obra_categoria oct on oct.octid = otp.octid
                    INNER JOIN par3.iniciativa_tipo_obra ini on ini.otpid = otp.otpid
                    WHERE
                    	ini.iniid = $iniid";

            $cabecalho = array('Etapa de Ensino', 'Categoria', 'Tipo de Obra');
            $listagem->addAcao('remove', 'removerTipoObra');
        }

        //Cabeçalho: código,descrição,situção
        $listagem->setCabecalho($cabecalho);
        // $listagem->setId('modalidade');
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('situacao', 'formata_status');
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function listarGrupo($iniid)
    {
        $sql = "SELECT           iig.iigid,
                                 itg.igrid,
                                 itg.igrdsc,
                                 itg.igrnome,

                                 TO_CHAR(itg.igrdtinclusao, 'dd/mm/YYYY')  AS igrdtinclusao
                                 FROM par3.iniciativa_itenscomposicao_grupo iig
                                 INNER JOIN par3.itenscomposicao_grupos itg ON itg.igrid =  iig.igrid
                                 WHERE  iig.iigsituacao = 'A'  AND iig.iniid = $iniid AND itcid IS NULL
                ";

        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código', 'Nomes', 'Descrição', 'Data Inclusão');
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        // $listagem->setId('modalidade');
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        //   $listagem->addCallbackDeCampo('itcdsc.itcsituacao', 'formata_status');
        $listagem->addAcao('remove', 'removerGrupo');
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

}

?>