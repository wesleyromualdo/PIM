<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 07/11/2016
 * Time: 11:07.
 */
class simec_Workflow
{
    protected $dadosAdicionais = array();
    protected $opcoesAdicionais = array();

// -- @todo fazer funcoes de acesso ou metodo magico de retorno
    protected $docid;
    protected $esdid;
    protected $esddsc;

    public function __construct($docid = null)
    {
        if ( !empty($docid) ) {
            $this->setDocid($docid);
        }
    }

    public function setDocid($docid)
    {
        $this->docid = $docid;
        $esd = wf_pegarEstadoAtual($docid);
        $this->esdid = $esd['esdid'];
        $this->esddsc = $esd['esddsc'];
        return $this;
    }

    public function getDocid()
    {
        return $this->docid;
    }

    public function getEsdid(){
        return $this->esdid;
    }

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
        $this->dadosAdicionais = $dados;
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
     * Altera o estado de um documento a partir de um estado de origem e um estado de destino.
     *
     * @param $docid             - Documento que será atualizado
     * @param $esdorigem         - Estado de origem da ação
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
