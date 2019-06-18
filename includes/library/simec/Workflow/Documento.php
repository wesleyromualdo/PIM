<?php

require_once APPRAIZ . "includes/workflow.php";

/**
 * Classe responsável por encapsular as funções de manipulação do workflow, responsáveis pela manipulação dos documentos
 * do Workflow.
 *
 * @example
 * <code>
 *      // -- EXEMPLO DE CRIAÇÃO DE UM DOCMENTO PARA O SISTEMA 'PACTO UNIVERSITÁRIO', TPDID 278, NO ESTADO
 *      // -- 'EM ELABORAÇÃO DO PLANO'. O PARÂMETRO DE ESTADO DO DOCUMENTO, O TERCEIRO PARÂMETRO, É OPCIONAL.
 *      $docid = (new Simec_Workflow_Documento)
 *              ->cadastraDocumento(278, 'Descrição do documento'[, 1844])
 *              ->getDocid();
 * </code>
 *
 * @example
 * <code>
 *      // -- EXEMPLO DE ALTERAÇÃO DO ESTADO DE UM DOCUMENTO
 *      $docid = 54086647;
 *      $esdidDestino = 1845;
 *      $comentario = '';
 *      $wf = new Simec_Workflow_Documento($docid);
 *
 *      // -- ADICIONA UM ARRAY DE INFORMAÇÕES ADICIONAIS
 *      $wf->setDadosAdicionais([]);
 *
 *      // -- ADICIONA UM ARRAY DE OPÇÕES ADICIONAIS
 *      $wf->setOpcoesAdicionais([]);
 *      $wf->alteraEstado($esdidDestino[, $comentario]);
 * </code>
 *
 * @package includes/library/simec
 * @filesource
 * @date 04 de Abril de 2017
 * @author Victor Martins Machado
 */
class Simec_Workflow_Documento
{
    /**
     * Variável responsável por agrupar os dados adicionais utilizados na tramitação
     * @var array
     */
    protected $dadosAdicionais = array();

    /**
     * Variável responsável por agrupar as opções adicionais utilizadas na tramitação
     * @var array
     */
    protected $opcoesAdicionais = array();

    /**
     * Variável usada para armazenar o id do documento
     * @var int
     */
    protected $docid;

    /**
     * Variável usada para armazenar o id do estado do documento
     * @var int
     */
    protected $esdid;

    /**
     * Variável usada para armazenar a descrição do estado do documento
     * @var string
     */
    protected $esddsc;

    public function __construct($docid = null)
    {
        if ( !empty($docid) ) {
            $this->setDocid($docid);
        }
    }

    /**
     * Seta o id do documento (docid)
     *
     * @param $docid
     * @return $this
     */
    public function setDocid($docid)
    {
        $this->docid = $docid;
        $esd = wf_pegarEstadoAtual($docid);
        $this->esdid = $esd['esdid'];
        $this->esddsc = $esd['esddsc'];
        return $this;
    }

    /**
     * Retorna o id do documento (docid)
     *
     * @return mixed - docid
     */
    public function getDocid()
    {
        return $this->docid;
    }

    /**
     * Retorna o id do estado do workflow (esdid)
     *
     * @return mixed - esdid
     */
    public function getEsdid(){
        return $this->esdid;
    }

    /**
     * Retorna a descrição do estado do workflow (esddsc)
     *
     * @return mixed - esddsc
     */
    public function getEsddsc(){
        return $this->esddsc;
    }

    /**
     * Pega os dados adicionais, que serão passados para a tramitação do workflow.
     * Esses dados são informados, normalmente, na abresentação da barra do workflow, segundo parâmetro da função
     * wf_desenhaBarraNavegacaoBootstrap.
     *
     * @param array $dados
     */
    public function setDadosAdicionais($dados = array())
    {
        $this->dadosAdicionais = serialize($dados);
        return $this;
    }

    /**
     * Pega as opções adicionais que alteram o funcionamento padrão da transição de estados.
     * => $opcoes['commit']: Define se o commit será chamado no final da transição. Padrão é true, executando o commit. Utilize
     * este parâmetro com o valor FALSE, quando precisar que a transição de estado aconteça dentro de uma transação, como,
     * por exemplo, um processamento em lote.
     *
     * @param array $opcoes
     */
    public function setOpcoesAdicionais($opcoes = array())
    {
        $this->opcoesAdicionais = $opcoes;
        return $this;
    }

    /**
     * Cadastra um documento, de acordo com o tipo de documento informado.
     *
     * @param $tpdid      - Tipo de documento desejado
     * @param $docdsc     - Descrição do documento
     * @param null $esdid - Estado de documento inicial, caso não seja informado será utilizado o estado inicial
     *                    cadastrado no estado
     *
     * @return $this
     */
    public function cadastraDocumento($tpdid, $docdsc, $esdid = null)
    {
        if (empty($tpdid)) {
            throw new Exception('Tipo de documento não informado');
        }

        $this->docid = wf_cadastrarDocumento($tpdid, $docdsc, $esdid);

        if (empty($this->docid)) {
            throw new Exception('Não foi possível cadastrar o documento');
        }

        return $this;
    }

    /**
     * Altera o estado de um documento.
     *
     * @param $esddestino        - Estado de destino da ação
     * @param string $comentario - Comentário da tramitação (opcional)
     *
     * @return $this
     */
    public function alteraEstado($esddestino, $comentario = '')
    {
        if (empty($this->esdid)){
            throw new Exception('Estado do documento não pode ser vazio');
        }

        if (empty($this->docid)) {
            throw new Exception('Documento não encontrado');
        }

        $acao = wf_pegarAcao($this->esdid, $esddestino);
        $aedid = $acao['aedid'];

        if (empty($aedid)) {
            throw new Exception('Ação não encontrada');
        }

        if (!wf_alterarEstado($this->docid, $aedid, $comentario, $this->dadosAdicionais, $this->opcoesAdicionais)) {
            $mensagem = wf_pegarMensagem();
            $mensagem = $mensagem ? $mensagem : 'Não foi possível alterar estado do documento.';
            throw new Exception($mensagem);
        }
        return $this;
    }

    /**
     * Retorna o estado atual do documento
     *
     * @param null $docid
     * @return mixed
     */
    public function retornaEstadoDocumento($docid = null)
    {
        $docid = empty($docid) ? $this->docid : $docid;
        $esd = wf_pegarEstadoAtual($docid);
        return $esd['esdid'];
    }
}
