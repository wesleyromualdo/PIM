<?php
/**
 * Classe de controle do  IniciativaModalidades
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   PHILIPE ALLAN ALMEIDA <philipe.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 27-06-2017
 * @link     no link
 */


/**
 * Par3_Controller_IniciativaModalidades
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 27-06-2017
 * @link     no link
 */
class Par3_Controller_IniciativaModalidades
{
    private $model;

    const url = 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaDetalhe&iniid=';

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaModalidades($_GET['imoid']);
        $this->modelHistorico = new Par3_Model_IniciativaModalidadesHistorico();
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
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $this->modelHistorico->gravarHistorico($this->model, 'CREATE');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::url . $_GET['iniid'], 'error');
        }

        if ($sucesso) {
            simec_redirecionar(self::url . $_GET['iniid'], 'success');
        }
        simec_redirecionar(self::url . $_GET['iniid'], 'error');
    }


    /**
     * Função excluir
     *
     * - grava os dados
     *
     */
    public function inativar($id)
    {
        $iniciativaModalidades = new Par3_Model_IniciativaModalidades($id);

        try {
            $iniciativaModalidades->imostatus = 'I';
            $iniciativaModalidades->salvar();
            $iniciativaModalidades->commit();
            $this->modelHistorico->gravarHistorico($iniciativaModalidades, 'DELETE');
            simec_redirecionar(self::url . $_GET['iniid'], 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar(self::url . $_GET['iniid'], 'error');
        }
    }


    /**
     * Função Validar
     *
     * Verificar se já existe as informações salvas para a mesma Iniciativa, ou seja, não poderá ser salvo o mesmo nível,
     * etapa, modalidade, desdobramento que já foi incluso uma vez para a mesma iniciativa, e caso não exista, salvará
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

        $nivid = $dados['nivid'];
        $etaid = $dados['etaid'];
        $modid = $dados['modid'];
        $desid = $dados['desid'];
        $iniid = $dados['iniid'];

        $imoid = $this->model->pegaUm("SELECT imoid
                              FROM par3.iniciativa_modalidades 
                              WHERE   nivid = $nivid AND 
                                      etaid = $etaid AND 
                                      modid = $modid AND 
                                      desid = $desid AND 
                                      imostatus = 'A' AND 
                                      iniid = $iniid");
        /**
         * Verificar se já existe as informações salvas
         */
        if ($imoid) {
            $retorno = [
                "status" => 'false',
                "mensagem" => "Já existe as informações salvas para a mesma Iniciativa, ou seja, não poderá ser salvo o mesmo nível, etapa, modalidade, desdobramento que já foi incluso uma vez para a mesma iniciativa."
            ];
        }
        
		// Verifica se já existe outra iniciativa com os mesmos campos
   /*     $modalIniciativa = new Par3_Model_Iniciativa($iniid);
        $arrayIniciativaValidacao = $dados;
        $erro = $modalIniciativa->validaInclusaoIniciativaDetalhe($arrayIniciativaValidacao, $opcoesValidacaoInclusao);

        if(array_filter($erro['erros'])){
        	$retorno = [
                "status" => 'false',
                "mensagem" => "Iniciativa informada já existe!"
            ];
        }   */
                
        exit(simec_json_encode($retorno));
    }


    /**
     * Listagem Modalidades
     *
     * @param null $arrPost
     */
    public function listar($iniid)
    {

        $sql = "
            SELECT imoid,
                ensino_nivel.nivdsc,
                ensino_etapa.etadsc,
                modalidade.moddsc,
                inides.desdsc
            FROM par3.iniciativa_modalidades inimod
            INNER JOIN par3.ensino_nivel
              ON (inimod.nivid = ensino_nivel.nivid)
            INNER JOIN par3.ensino_etapa
              ON (inimod.etaid = ensino_etapa.etaid)
            INNER JOIN par3.modalidade
              ON (inimod.modid = modalidade.modid)
            INNER JOIN par3.iniciativa_desdobramento inides
              ON (inimod.desid = inides.desid)
            WHERE imostatus = 'A' AND 
                  iniid = $iniid
        ";

        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Nível', 'Etapa', 'Modalidade', 'Desdobramento');
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('imoid');
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('remove', 'removerModalidades');
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }


}

?>