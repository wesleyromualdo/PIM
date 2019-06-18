<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_documento_historico.
 *
 * @version $Id$
 * @since 2017.07.05
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Obra_tipo_documentoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Philipe Allan Almeida <philipe.almeida@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_tipo_documentoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_tipo_documentoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hotid chave primária da tabelaobra_tipo_documento_historico
 * @property  $hotcpf CPF do respnsável pela alteração
 * @property  $hotacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hotdtcriacao Data de criação do log
 * @property int $otdid Chave primária da tabela - default: nextval('par3.obra_tipo_documento_otdid_seq'::regclass)
 * @property int $extid FK da Extensão de Arquivo (public.extensao_arquivo)
 * @property string $otddsc Descrição do Documentos Anexo
 * @property string $otdajuda Texto de Ajuda
 * @property string $otdsituacao Situação com as opções A = Ativo e I = Inativo
 * @property string $otdstatus Status do registro A = ativo e I = inativo - default: 'A'::bpchar
 * @property int $otdtamanho_minimo Quantidade Mínima para o tamanho do arquivo em Kb.
 * @property int $otdtamanho_maximo Quantidade Máxima do tamaho do arquivo em Kb.
 * @property int $otdqtd_minima Quantidade mínima de arquivos
 * @property int $otdqtd_maximo Quantidade máxima de arquivos.
 */
class Par3_Model_ObraTipoDocumentoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_documento_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hotid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otdid' => array('tabela' => 'par3.obra_tipo_documento', 'pk' => 'otdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hotid' => null,
        'hotcpf' => null,
        'hotacao' => null,
        'hotdtcriacao' => null,
        'otdid' => null,
        'arqid' => null,
        'otddsc' => null,
        'otdajuda' => null,
        'otdsituacao' => null,
        'otdstatus' => null,
        'otdtamanho_minimo' => null,
        'otdtamanho_maximo' => null,
        'otdqtd_minima' => null,
        'otdqtd_maximo' => null,
        'otdtodas_extensoes' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraTipoDocumento $model,$acao)
    {

        if ($acao == 'CREATE'){
            $this->arAtributos['otdstatus'] = 'A';
        }

        if ($acao == 'DELETE'){
            $this->arAtributos['otdstatus'] = 'I';
        }

        if ($acao == 'UPDATE'){
            $this->arAtributos['otdstatus'] = 'U';
        }

        $this->arAtributos['hotcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hotacao'] = $acao;
        $this->arAtributos['hotdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['otdid'] = $model->otdid;
        $this->arAtributos['otddsc'] = $model->otddsc;
        $this->arAtributos['arqid'] = $model->arqid;
        $this->arAtributos['otdajuda'] = $model->otdajuda;
        $this->arAtributos['otdsituacao'] = $model->otdsituacao;
        $this->arAtributos['otdtamanho_minimo'] = $model->otdtamanho_minimo;
        $this->arAtributos['otdtamanho_maximo'] = $model->otdtamanho_maximo;
        $this->arAtributos['otdqtd_minima'] = $model->otdqtd_minima;
        $this->arAtributos['otdqtd_maximo'] = $model->otdqtd_maximo;
        $this->arAtributos['otdtodas_extensoes'] = $model->otdtodas_extensoes;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}
