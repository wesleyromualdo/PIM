<?php
/**
 * Classe de mapeamento da entidade par.situacaoopc.
 *
 * @version $Id$
 * @since 2018.07.04
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par_Model_Situacaoopc: Tabela responsável por armazenar o relacionamento do processo com o DOCID do fluxo da situação da obrigatoriadade de prestar de contas
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par_Model_Situacaoopc($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par_Model_Situacaoopc($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $sopid Primary key da tabela - default: nextval('par.situacaoopc_sopid_seq'::regclass)
 * @property int $prpid Id do documento do workflow
 * @property int $docid 
 * @property string $sopstatus Status do registro A = Ativo / I = Inativo - default: 'A'::bpchar
 */
class Par_Model_Situacaoopc extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.situacaoopc';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'sopid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'prpid' => array('tabela' => 'par.processopar', 'pk' => 'prpid'),
    );
    
    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     * @param Array $arrParams            
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosSemFluxo($arrParams)
    {
        // Recupera parametros
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
        
        // 'par.situacaoopc';
        /*
         * RN = Seleciona todos prpids que não possuam registro na tabela par.situacaoopc
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
                prp.prpid not in (select distinct prpid from par.situacaoopc)
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
    
            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_OBRIGATORIEDADE_DE_PRESTAR_CONTAS, "Cadastro de documento do Fluxo de Obrigatoriedade de prestar contas - Via acompanhamento", ESDID_OPC_ADIMPLENTE);
            $prpid = $v['prpid'];
            if (($prpid != '') && ($docidFluxo != '')) {
                $arrValores[] = "({$prpid}, {$docidFluxo})";
            }
        }
    
        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO par.situacaoopc
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
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'sopid' => null,
        'prpid' => null,
        'docid' => null,
        'sopstatus' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'sopid' => array('Digits'),
            'prpid' => array('allowEmpty' => true,'Digits'),
            'docid' => array('allowEmpty' => true,'Digits'),
            'sopstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
