<?php
/**
 * Classe de mapeamento da entidade par.situacaoprestacaocontas.
 *
 * @version $Id$
 * @since 2018.07.04
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par_Model_Situacaoprestacaocontas: Tabela responsável por armazenar o relacionamento do processo com o DOCID do fluxo da situação da prestação de contas
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *        
 * @example <code>
 *          // -- Consultando registros
 *          $model = new Par_Model_Situacaoprestacaocontas($valorID);
 *          var_dump($model->getDados());
 *         
 *          // -- Alterando registros
 *          $valores = ['campo' => 'valor'];
 *          $model = new Par_Model_Situacaoprestacaocontas($valorID);
 *          $model->popularDadosObjeto($valores);
 *          $model->salvar(); // -- retorna true ou false
 *          $model->commit();
 *          </code>
 *         
 * @property int $spcid Primary key da tabela - default: nextval('par.situacaoprestacaocontas_spcid_seq'::regclass)
 * @property int $prpid Id do documento do workflow
 * @property int $docid
 * @property string $spcstatus Status do registro A = Ativo / I = Inativo - default: 'A'::bpchar
 */
class Par_Model_Situacaoprestacaocontas extends Modelo
{

    /**
     *
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.situacaoprestacaocontas';

    /**
     *
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'spcid'
    );

    /**
     *
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array(
            'tabela' => 'workflow.documento',
            'pk' => 'docid'
        ),
        'prpid' => array(
            'tabela' => 'par.processopar',
            'pk' => 'prpid'
        )
    );

    /**
     *
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'spcid' => null,
        'prpid' => null,
        'docid' => null,
        'spcstatus' => null
    );

    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams            
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosSemFluxo($arrParams)
    {
        // Recupera parametros
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
        
        // 'par.documentoparprestacaodecontas';
        /*
         * RN = Seleciona todos prpids que não possuam registro na tabela par.situacaoprestacaocontas
         * RN2 = Que não estajam contemplados pelo SIGPC
         */
        $sql = "
    	   SELECT DISTINCT 
            	prp.prpid,
            	dpp.dppid,
            	doc.esdid as esdid_doc_pc
            FROM 
            	par.processopar prp
            LEFT JOIN par.documentoparprestacaodecontas dpp ON dpp.prpid = prp.prpid
            LEFT JOIN workflow.documento doc ON doc.docid = dpp.docid
            WHERE 
            	prpstatus = 'A' 
            AND 
            	prp.prpid not in (select distinct prpid from par.situacaoprestacaocontas)
            AND 
            	prp.prpid not in (select distinct prpid from par.termossigpcpar WHERE prpid is not null)
            {$offset}
            {$limite}
        ";
        
        $arrProcessos = $this->carregar($sql);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou = (is_array($arrProcessos)) ? TRUE : FALSE;
        $registros = (is_array($arrProcessos)) ? $arrProcessos : Array();
        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'cadastroRegistro' => $registros
        );
        return $arrReturn;
    }

    /**
     */
    public function retornaSituacaoPelaFase($esdid = NULL)
    {
        $arrEquivalencia = Array(
            ESDID_AGUARDANDO_ENVIO_PROPONENTE => ESDID_SPC_NAO_ENVIADA,
            ESDID_AGUARDANDO_ANALISE_CONSELHO => ESDID_SPC_ENVIADA_AO_CONTROLE_SOCIAL
        );
        
        if ($esdid != NULL) {
            $resultado = $arrEquivalencia[$esdid];
            
            if ($resultado != '') {
                return $resultado;
            } else {
                return FALSE; // @TODO finalizar os demais fluxos
            }
        }
    }

    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams            
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function insereRegistrosInexistente($arrRegistros)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
        
        $arrValores = Array();
        foreach ($arrRegistros as $k => $v) {
            $docidFluxo = '';
            $prpid = '';
            $esdidFase = $v['esdid_doc_pc'];
            if ($esdidFase != '') {
                $esdidAtual = $this->retornaSituacaoPelaFase($esdidFase);
                $esdidAtual = ($esdidAtual) ? $esdidAtual : ESDID_SPC_NAO_ENVIADA;
            } else {
                $esdidAtual = ESDID_SPC_NAO_ENVIADA;
            }
            $prpid = $v['prpid'];
            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_SITUACAO_PRESTACAO_CONTAS, "Cadastro de documento do Fluxo de Situação da Prestação de Contas - Via acompanhamento", $esdidAtual);
            
            if (($prpid != '') && ($docidFluxo != '')) {
                $arrValores[] = "({$prpid}, {$docidFluxo})";
            }
        }
        
        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO par.situacaoprestacaocontas
                    (prpid, docid)
                VALUES
                ";
            $sql .= implode(",
                            ", $arrValores);
            
            $this->executar($sql);
            return $this->commit();
        }
        
        return FALSE;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados            
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'spcid' => array(
                'Digits'
            ),
            'prpid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'docid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'spcstatus' => array(
                'allowEmpty' => true,
                new Zend_Validate_StringLength(array(
                    'max' => 1
                ))
            )
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        
        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }
}
