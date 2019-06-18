<?php
/**
 * Classe de mapeamento da entidade sase.assessoramentoestado_historico.
 *
 * @version $Id$
 * @since 2017.09.28
 */

/**
 * Sase_AssessoramentoEstadoModel_AssessoramentoestadoHistorico: sem descricao
 *
 * @package Sase\AssessoramentoEstadoModel
 * @uses Simec\Db\Modelo
 * @author Marilúcia Cardozo De Queiroz <marilucia.queiroz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_AssessoramentoEstadoModel_AssessoramentoestadoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_AssessoramentoEstadoModel_AssessoramentoestadoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hasid chave primária da tabelaassessoramentoestado_historico
 * @property  $hascpf CPF do respnsável pela alteração
 * @property  $hasacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hasdtcriacao Data de criação do log
 * @property int $aseadesao 
 * @property int $aseleipne 
 * @property string $asestatus  - default: 'A'::bpchar
 * @property int $docid 
 * @property int $docid 
 * @property string $usucpf 
 * @property int $stacod 
 * @property string $estuf 
 * @property int $aseid  - default: nextval('sase.assessoramentoestado_aseid_seq'::regclass)
 */
class Sase_AssessoramentoEstado_Historico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.assessoramentoestado_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hasid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'aseid' => array('tabela' => 'sase.assessoramentoestado', 'pk' => 'aseid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hasid' => null,
        'hascpf' => null,
        'hasacao' => null,
        'hasdtcriacao' => null,
        'aseadesao' => null,
        'aseleipne' => null,
        'asestatus' => null,
        'docid' => null,
        'docid' => null,
        'usucpf' => null,
        'stacod' => null,
        'estuf' => null,
        'aseid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Sase_AssessoramentoEstadoModel_Assessoramentoestado $model,$acao)
    {
    
        $this->arAtributos['hascpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hasacao'] = $acao;
        $this->arAtributos['hasdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['aseadesao'] = $model->aseadesao;
        $this->arAtributos['aseleipne'] = $model->aseleipne;
        $this->arAtributos['asestatus'] = $model->asestatus;
        $this->arAtributos['docid'] = $model->docid;
        $this->arAtributos['docid'] = $model->docid;
        $this->arAtributos['usucpf'] = $model->usucpf;
        $this->arAtributos['stacod'] = $model->stacod;
        $this->arAtributos['estuf'] = $model->estuf;
        $this->arAtributos['aseid'] = $model->aseid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
    
    /**
     * MontaListaHistorico para a função monta_lista da classe_simec.
     *
     * @param array $id, $uf
     * @return table com registros (upload de arquivos) alterados na tela Assistência Técnica Estado
     */
    public function montaListaHistorico( $aseid, $estuf ){
        $this->arAtributos['aseid'] = $aseid;
            
        $aseleipne = "'<a style=\"cursor:pointer\" onclick=\"downloadAnexo( \'aseleipne\',' || ahe.aseleipne || ' )\"><img src=\"/imagens/sase/direction.png\" title=\"Download Lei PNE - '|| pa2.arqdescricao ||' \"/></a>'";
        $aseadesao = "'<a style=\"cursor:pointer\" onclick=\"downloadAnexo(\'aseadesao\',' || ahe.aseadesao || '  )\"><img src=\"/imagens/sase/direction.png\" title=\"Download Adesão - '|| pa1.arqdescricao ||' \"/></a>'";
                
        $sql = "
            SELECT
                to_char( ahe.hasdtacao, 'DD/MM/YY HH24:MI:SS' ) as hasdtacao,
                TO_CHAR(ahe.hascpf::BIGINT, '000\"-\"000\"-\"000\"-\"00')  as hascpf,
                u.usunome,
                CASE
                  when ahe.aseleipne is not null then {$aseleipne}
                  else ' -- '
                END as aseleipne,
                CASE
                  when ahe.aseadesao is not null then {$aseadesao}
                  else ' -- '
                END as aseadesao
            FROM " . $this->stNomeTabela . " ahe
            JOIN seguranca.usuario u ON u.usucpf = ahe.hascpf
            JOIN public.arquivo pa1 ON pa1.arqid = ahe.aseadesao
            JOIN public.arquivo pa2 ON pa2.arqid = ahe.aseleipne
            WHERE aseid = ".$aseid." AND ahe.estuf = '".$estuf."'";
        return $sql;
    }

}
