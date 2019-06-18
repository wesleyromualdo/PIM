<?php
/**
 * Classe de controle do  IniciativaIndicadores
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   PHILIPE ALLAN ALMEIDA <philipe.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 27-06-2017
 * @link     no link
 */


/**
 * Par3_Controller_IniciativaIndicadores
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 27-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaIndicadores
{
    private $model;
    const url = 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&iniid=';

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaIndicadores($_GET['idiid']);
        $this->modelHistorico = new Par3_Model_IniciativaIndicadoresHistorico();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;

        $this->model->popularDadosObjeto();

        try {
            $camposNull = array();

            //Faz o tratamento dos campos em branco
            foreach ($_POST as $chave => $valor) {
                if ($valor == '' && $chave) {
                    $dados[$chave] = null;
                    $camposNull[] = $chave;

                }
            }
            $sucesso = $this->model->salvar($boAntesSalvar = true, $boDepoisSalvar = true, $camposNull);
            $this->model->commit();
            $this->modelHistorico->gravarHistorico($this->model, 'CREATE');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::url.$_GET['iniid'], 'error');
        }

        if ($sucesso) {
            simec_redirecionar(self::url.$_GET['iniid'], 'success');
        }
        simec_redirecionar(self::url.$_GET['iniid'], 'error');
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($id)
    {
        $iniciativaIndicadores = new Par3_Model_IniciativaIndicadores($id);
        try {
            $iniciativaIndicadores->idistatus = 'I';
            $iniciativaIndicadores->salvar();
            $iniciativaIndicadores->commit();
            $this->modelHistorico->gravarHistorico($iniciativaIndicadores, 'DELETE');
            simec_redirecionar(self::url.$_GET['iniid'], 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::url.$_GET['iniid'], 'error');
        }
    }

    /**
     * Função Validar
     *
     * Verificar se já existe as informações salvas para a mesma Iniciativa, ou seja, não poderá ser salvo o mesmo Dimensão,
     * Área, Indicador, Componentes do Indicador que já foi incluso uma vez para a mesma iniciativa, e caso não exista, salvará
     * o registro;
     *
     */
    public function validar($dados)
    {
        ob_clean();

        parse_str($dados['dados'], $dados);

        $retorno = [
            "status" => true,
            "mensagem" => "Validado com sucesso!"
        ];

        $dimid = !isset($dados['dimid'])||$dados['dimid']==''?'IS NULL':" = ".$dados['dimid'];
        $areid = !isset($dados['areid'])||$dados['areid']==''?'IS NULL':" = ".$dados['areid'];
        $indid = !isset($dados['indid'])||$dados['indid']==''?'IS NULL':" = ".$dados['indid'];
        $crtid = !isset($dados['crtid'])||$dados['crtid']==''?'IS NULL':" = ".$dados['crtid'];
        $iniid = $dados['iniid'];

        $sql = "SELECT idiid
                FROM par3.iniciativa_indicadores
                WHERE
                dimid $dimid AND
                areid $areid AND
                indid $indid AND
                crtid $crtid AND
                idistatus = 'A'AND
                iniid = $iniid";

        $idiid = $this->model->pegaUm($sql);

        /**
         * Verificar se já existe as informações salvas
         */
        if ($idiid != '') {
            $retorno = [
                "status" => 'false',
                "mensagem" => "Já existe as informações salvas para a mesma Iniciativa, ou seja, não poderá ser salvo o mesmo Dimensão, Área, Indicador e Componentes do Indicador que já foi incluso uma vez para a mesma iniciativa."
            ];
        }

        // Verifica se já existe outra iniciativa com os mesmos campos
//         $modalIniciativa = new Par3_Model_Iniciativa($iniid);
//         $arrayIniciativaValidacao = $dados;
//         $erro = $modalIniciativa->validaInclusaoIniciativaDetalhe($arrayIniciativaValidacao, $opcoesValidacaoInclusao);

//         if(array_filter($erro['erros'])){
//         	$retorno = [
//         	"status" => 'false',
//         	"mensagem" => "Iniciativa informada já existe!"
//         			];
//         }

        exit(simec_json_encode($retorno));
    }

    /**
     * Listagem Indicadores
     *
     * @param null $arrPost
     */
    public function listar($iniid)
    {
        $sql = "
                SELECT  idiid,
                      (CASE
                           WHEN itrid = 1 THEN dimcod||' - '||dimdsc||' - '||'Estadual'
                           WHEN itrid = 3 THEN dimcod||' - '||dimdsc||' - '||'Distrital'
                           ELSE dimcod||' - '||dimdsc||' - '||'Municipal'
                      END) as descricao,
                        area.aredsc,
                        indicador.inddsc,
                        criterio.crtdsc
                FROM par3.iniciativa_indicadores inind
                LEFT JOIN par3.dimensao
                  ON (inind.dimid = dimensao.dimid)
                LEFT JOIN par3.area
                  ON (inind.areid = area.areid)
                LEFT JOIN par3.indicador
                  ON (inind.indid = indicador.indid)
                LEFT JOIN par3.criterio
                  ON (inind.crtid = criterio.crtid)
                WHERE idistatus = 'A' AND
                      iniid = $iniid
                ";

        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Dimensão', 'Área', 'Indicador', 'Componentes do Indicador');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('modalidade');
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('modsituacao', 'formata_status');
        $listagem->addAcao('remove', 'removerIndicadores');
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

}

?>