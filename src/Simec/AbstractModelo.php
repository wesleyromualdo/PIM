<?php
namespace Simec;

/**
 * Classe de representação para um modelo de acesso ao banco de dados
 */
class AbstractModelo
{

    /**
     * Modelo padrão do simec antigo
     * @var \Modelo
     */
    private $modelo;

    /**
     * Objeto de dados padrão da modelo
     * @var \Simec\AbstractDado 
     */
    public $dados;

    /**
     * Método construtor
     */
    public function __construct(array $ar = null)
    {
        $this->modelo = new \Modelo();
        $dadoClassname = $this->buscarNamespaceDado();
        if (class_exists($dadoClassname)) {
            $this->dados = new $dadoClassname($ar);
        }
    }

    /**
     * Retorna o namespace da classe de dados referente à esta model
     * @return string
     */
    private function buscarNamespaceDado()
    {
        $name = explode('\\', (new \ReflectionClass($this))->getName());
        $name[(count($name) -2)] = 'Dado';
        return implode('\\', $name);
    }

    /**
     * Criador da chamada para os métodos antigos do simec
     * @param type $name
     * @param type $arguments
     * @return type
     */
    public function __call($name, $arguments)
    {
        return call_user_func_array([$this->modelo, $name], $arguments);
    }

    /**
     * Matando método que não pertence à esta camada
     * @throws Excecao
     */
    protected function gerarExcel()
    {
        throw new Excecao('Por favor utilize uma visualização para fazer isto! Tenha um pouco de dignidade no seu código.');
    }

    /**
     * Carrega uma coleção de dados
     * @param string $sql
     * @param \Simec\Colecao $colecao
     * @param \Simec\AbstractDado $valueObject
     * @return \Simec\Colecao
     */
    protected function carregarColecao($sql, Colecao $colecao, AbstractDado $valueObject)
    {
        $res = $this->modelo->executar($sql);
        while (($tupla = $this->modelo->proximo())) {
            $obj = clone $valueObject;
            $colecao[] = $obj->carregar($tupla);
        }
        return $colecao;
    }

    /**
     * Retorna o comando de leitura
     * @param \Simec\AbstractDado $dado
     * @return string
     * @throws Excecao\Dado
     */
    protected function montarComandoLer(AbstractDado $dado)
    {
        $ar = $dado->mapear();
        $pk = [];
        foreach ($ar['campos'] as $campo) {
            $valor = $this->tratarValor($campo, $campo['valor']);
            if ($campo['chave']) {
                $pk[] = " {$campo['campo']}={$valor}";
            }
        }

        if (!count($pk)) {
            throw new Excecao\Dado('Doc @chave : Nenhum campo chave foi definido na classe.');
        }
        return sprintf("%s where %s ", $this->montarComandoLerTodos($dado), implode(' and ', $pk));
    }

    /**
     * Retorna o comando de exclusão para o AbstractDado
     * @param \Simec\AbstractDado $dado
     * @return string
     * @throws Excecao\Dado
     */
    protected function montarComandoExcluir(AbstractDado $dado)
    {
        $ar = $dado->mapear();
        $pk = [];
        foreach ($ar['campos'] as $campo) {
            $valor = $this->tratarValor($campo, $campo['valor']);
            if ($campo['chave']) {
                $pk[] = " {$campo['campo']}={$valor}";
            }
        }
        if (!count($pk)) {
            throw new Excecao\Dado('Doc @chave : Nenhum campo chave foi definido na classe.');
        }
        return sprintf("delete from %s where %s ", $ar['tabela'], implode(' and ', $pk));
    }

    /**
     * Retorna o comando de alteração para o AbstractDado
     * @param \Simec\AbstractDado $dado
     * @return string
     * @throws Excecao\Dado
     */
    protected function montarComandoAtualizar(AbstractDado $dado)
    {
        $ar = $dado->mapear();
        $campos = $pk = [];
        foreach ($ar['campos'] as $campo) {
            $valor = $this->tratarValor($campo, $campo['valor']);
            if ($campo['chave']) {
                if ($valor === AbstractDado::nulo) {
                    $pk[] = " {$campo['campo']} is null";
                } else {
                    $pk[] = " {$campo['campo']}={$valor}";
                }
            }
            if ($valor === null) {
                continue;
            }
            if ($valor === AbstractDado::nulo) {
                $campos[] = " {$campo['campo']}=null";
            } else {
                $campos[] = " {$campo['campo']}={$valor}";
            }
        }
        if (!count($pk)) {
            throw new Excecao\Dado('Doc @chave : Nenhum campo chave foi definido na classe.');
        }
        return sprintf("update %s set %s where %s ", $ar['tabela'], implode(',', $campos), implode(' and ', $pk));
    }

    /**
     * Retorna o comando de inclusão para o AbstractDado
     * @param \Simec\AbstractDado $dado
     * @return string
     */
    protected function montarComandoIncluir(AbstractDado $dado)
    {
        $ar = $dado->mapear();
        $campos = $valores = $pk = [];
        foreach ($ar['campos'] as $campo) {
            $camposReturning[] = $campo['campo'];
            $valor = $this->tratarValor($campo, $campo['valor']);
            if ($valor === null) {
                continue;
            }
            $campos[] = $campo['campo'];
            $valores[] = ($valor == AbstractDado::nulo) ? 'null' : $valor;
        }
        return sprintf('insert into %s (%s) values(%s) returning %s'
            , $ar['tabela'], implode(',', $campos), implode(',', $valores), implode(',', $camposReturning));
    }

    /**
     * Retorna o caomando de leitura para o AbstractDado
     * @param \Simec\AbstractDado $dado
     * @param boolean $alias
     * @return string
     */
    protected function montarComandoLerTodos(AbstractDado $dado, $alias = true)
    {
        $ar = $dado->mapear();
        $campos = $atributos = [];
        if ($alias) {
            foreach ($ar['campos'] as $campo) {
                $campos[] = " {$campo['campo']} as \"{$campo['atributo']}\"";
            }
            return sprintf('select %s from %s ', implode(',', $campos), $ar['tabela']);
        }
        return sprintf('select * from %s ', $ar['tabela']);
    }

    /**
     * Método que cria a elaboração do filtro da sql
     * @param array $campo configuração do campo
     * @param \Simec\Filtro $valor
     * @return string
     */
    protected function elaborarFiltro($campo, $valor)
    {
        $montarFiltro = function($campo, $filtro) {
            switch ($filtro->operador()) {
                case Filtro::nulo:
                    return sprintf(" %s is null %s ", $campo['campo'], $filtro->conjuncao());
                case Filtro::dominio:
                    $valores = implode(',', array_map(function($valor) use ($campo) {
                            return AbstractModelo::tratarValor($campo, $valor);
                        }, $filtro->valor()));
                    return sprintf(" %s in(%s) %s ", $campo['campo'], $valores, $filtro->conjuncao());
                case Filtro::entre:
                    $valor1 = AbstractModelo::tratarValor($campo, $filtro->valor1());
                    $valor2 = AbstractModelo::tratarValor($campo, $filtro->valor2());
                    return sprintf(" %s between %s and %s %s ", $campo['campo'], $valor1, $valor2, $filtro->conjuncao());
                default:
                    $valor = AbstractModelo::tratarValor($campo, $filtro->valor());
                    return sprintf(" %s %s %s %s ", $campo['campo'], $filtro->simbolo(), $valor, $filtro->conjuncao());
            }
        };
        if ($valor instanceof Filtro) {
            return $montarFiltro($campo, $valor);
        }
        return sprintf(" %s = %s and ", $campo['campo'], AbstractModelo::tratarValor($campo, $valor));
    }

    /**
     * Trata o valor pelo tipo para uso nas sqls
     * @param array $campo configuração do campo
     * @param string $valor
     * @return string
     */
    protected static function tratarValor($campo, $valor)
    {
        if ($valor === null) {
            return null;
        }
        $valor = str_replace("'", "''", $valor);
        switch (true) {
            case $campo['tipo'] == 'numerico':
                return $valor;
            default:
                return "'{$valor}'";
        }
    }

    /**
     * Remove caso exista aquela conjunção que sobra
     * @param string $sql
     * @return string
     */
    protected function removerConjuncaoFinal($sql)
    {
        if (preg_match('/(.*)\s(and|or)\s*$/', $sql, $match)) {
            return $match[1];
        }
        return $sql;
    }

    /**
     * Preenche um objeto com os dados
     * @param \Simec\AbstractDado $valueObject
     * @param string $sql
     * @return \Simec\AbstractDado
     */
    public function lerObjeto(AbstractDado $valueObject, $sql = null)
    {
        if ($sql) {
            $res = $this->modelo->executar($sql);
        } else {
            $res = $this->modelo->executar($this->montarComandoLer($valueObject));
        }
        if (($tupla = $this->modelo->proximo())) {
            $valueObject->carregar($tupla);
        }
        return $valueObject;
    }

    /**
     * Retorna a leitura de objetos similares ao objeto passado
     * @param \Simec\AbstractDado $dado
     * @param \Simec\Pagina $pagina
     * @return \Simec\Colecao
     */
    public function lerSimilares(AbstractDado $dado, Pagina $pagina = null)
    {
        $pagina = $pagina ? $pagina : new Pagina();
        $ar = $dado->mapear();
        $campos = [];
        $stFiltro = '';
        $temFiltro = false;
        foreach ($ar['campos'] as $campo) {
            if ($campo['valor']) {
                $temFiltro = true;
                $stFiltro .= $this->elaborarFiltro($campo, $campo['valor']);
            }
        }
        $stFiltro = $this->removerConjuncaoFinal($stFiltro);
        $sql = $this->montarComandoLerTodos($dado);
        if ($temFiltro) {
            $sql = sprintf("%s where %s ", $this->montarComandoLerTodos($dado), $stFiltro);
        }
        $valueObject = clone $dado;
        foreach ($valueObject as $attr => $value) {
            $valueObject->{$attr} = null;
        }
        if($pagina->numero()){
            $sql = sprintf(
                'select * from (%s) selecao limit %s offset %', 
                $sql,
                $pagina->tamanho(), 
                $pagina->tamanho() * $pagina->numero()
                );
        }
        return $this->carregarColecao($sql, new Colecao(), $dado);
    }

    /**
     * Registra um novo dado no banco
     * @param \Simec\AbstractDado $dado
     * @return $this
     */
    public function incluir(AbstractDado $dado = null)
    {
        switch (true) {
            case $dado:
                $this->modelo->executar($this->montarComandoIncluir($dado));
                $dado->carregar($this->modelo->proximo());
            break;
            case $this->dados instanceof AbstractDado :
                $this->modelo->executar($this->montarComandoIncluir($this->dados));
                $this->dados->carregar($this->modelo->proximo());
            break;
        }
        return $this;
    }

    /**
     * Remove um dado no banco
     * @param \Simec\AbstractDado $dado
     * @return $this
     */
    public function excluir(AbstractDado $dado = null)
    {
        switch (true) {
            case $dado:
                $this->modelo->executar($this->montarComandoExcluir($dado));
                $dado->carregar($this->modelo->proximo());
                break;
            case $this->dados instanceof AbstractDado :
                $this->modelo->executar($this->montarComandoExcluir($this->dados));
                $this->dados->carregar($this->modelo->proximo());
                break;
        }
        return $this;
    }

    /**
     * Atualiza um dado no banco, executa um update em uma tabela do banco
     * @param \Simec\AbstractDado $dado
     * @return $this
     */
    public function atualizar(AbstractDado $dado = null)
    {
        switch (true) {
            case $dado:
                $this->modelo->executar($this->montarComandoAtualizar($dado));
                $dado->carregar($this->modelo->proximo());
                break;
            case $this->dados instanceof AbstractDado :
                $this->modelo->executar($this->montarComandoAtualizar($this->dados));
                $this->dados->carregar($this->modelo->proximo());
                break;
        }
        return $this;
    }

    /**
     * Inclui uma coleção de registros no banco de dados
     * @param \Simec\Colecao $colecao
     * @return $this
     */
    public function incluirColecao(Colecao $colecao)
    {
        foreach ($colecao as $dado) {
            $this->incluir($dado);
        }
        return $this;
    }

    /**
     * Exclui uma coleção de registros no banco de dados
     * @param \Simec\Colecao $colecao
     * @return $this
     */
    public function excluirColecao(Colecao $colecao)
    {
        foreach ($colecao as $dado) {
            $this->excluir($dado);
        }
        return $this;
    }

    /**
     * Atualiza uma coleção de registros no banco de dados
     * @param \Simec\Colecao $colecao
     * @return $this
     */
    public function atualizarColecao(Colecao $colecao)
    {
        foreach ($colecao as $dado) {
            $this->atualizar($dado);
        }
        return $this;
    }

    public function lerEstruturaBanco($tabela)
    {
        return $this->modelo->carregar(<<<sql
			select distinct
				tabela.*,
				case when tabela.tipo_de_dado <> 'numerico' then
					tamanho
				else
					tamanho/65536
				end as tamanho,
				pk.campo_pk,
				fk.constraint,
				fk.esquema_fk,
				fk.tabela_fk,
				fk.campo_fk,
				uk.unique_key
			from 
				(SELECT 
					n.nspname as esquema, 
					c.relname as tabela, 
					a.attname as campo,
					case 
						when a.attnotnull = 't' then 'not null'
						when a.attnotnull = 'f' then ''
					end as obrigatorio,
					format_type(t.oid, null) as tipo,
					case 
						when format_type(t.oid, null) = 'bigint' then 'numerico'
						when format_type(t.oid, null) = 'bigserial' then 'numerico'
						when format_type(t.oid, null) = 'bit' then 'numerico'
						when format_type(t.oid, null) = 'bit varying' then 'numerico'
						when format_type(t.oid, null) = 'boolean' then 'numerico'
						when format_type(t.oid, null) = 'character varying' then 'texto'
						when format_type(t.oid, null) = 'character' then 'texto'
						when format_type(t.oid, null) = 'date' then 'data'
						when format_type(t.oid, null) = 'double precision' then 'numerico'
						when format_type(t.oid, null) = 'integer' then 'numerico'
						when format_type(t.oid, null) = 'interval' then 'numerico'
						when format_type(t.oid, null) = 'money' then 'numerico'
						when format_type(t.oid, null) = 'numeric' then 'numerico'
						when format_type(t.oid, null) = 'real' then 'numerico'
						when format_type(t.oid, null) = 'smallint' then 'numerico'
						when format_type(t.oid, null) = 'serial' then 'numerico'
						when format_type(t.oid, null) = 'text' then 'texto'
						when format_type(t.oid, null) = 'time with time zone' then 'data'
						when format_type(t.oid, null) = 'timestamp' then 'data'
						when format_type(t.oid, null) = 'timestamp with time zone' then 'data'
						when format_type(t.oid, null) = 'timestamp without time zone' then 'data'
						else coalesce(format_type(t.oid, null), 'tdata')
					end as tipo_de_dado					
					, case 
						when a.attlen >= 0 then a.attlen
						else (a.atttypmod-4)
					  END AS tamanho
					, cm.description as descricao
				FROM	pg_attribute a
					inner join pg_class c
						on a.attrelid = c.oid
					inner join pg_namespace n
						on c.relnamespace = n.oid
					inner join pg_type t
						on a.atttypid = t.oid
					left join pg_description cm
						on a.attrelid::oid = cm.objoid::oid
						and attnum = cm.objsubid
				WHERE 
					c.relkind = 'r'                       -- no indices
					and n.nspname not like 'pg\\_%'       -- no catalogs
					and n.nspname != 'information_schema' -- no information_schema
					and a.attnum > 0                      -- no system att's
					and not a.attisdropped                -- no dropped columns
				) as tabela 
				left join 
				(SELECT
					pg_namespace.nspname AS esquema,
					pg_class.relname AS tabela,
					pg_attribute.attname  AS campo_pk
				FROM 
					pg_class
					JOIN pg_namespace ON pg_namespace.oid=pg_class.relnamespace AND
					pg_namespace.nspname NOT LIKE E'pg_%'
					JOIN pg_attribute ON pg_attribute.attrelid=pg_class.oid AND
					pg_attribute.attisdropped='f'
					JOIN pg_index ON pg_index.indrelid=pg_class.oid AND
					pg_index.indisprimary='t' AND (
						pg_index.indkey[0]=pg_attribute.attnum OR
						pg_index.indkey[1]=pg_attribute.attnum OR
						pg_index.indkey[2]=pg_attribute.attnum OR
						pg_index.indkey[3]=pg_attribute.attnum OR
						pg_index.indkey[4]=pg_attribute.attnum OR
						pg_index.indkey[5]=pg_attribute.attnum OR
						pg_index.indkey[6]=pg_attribute.attnum OR
						pg_index.indkey[7]=pg_attribute.attnum OR
						pg_index.indkey[8]=pg_attribute.attnum OR
						pg_index.indkey[9]=pg_attribute.attnum
					)
				) as pk
				on (
					tabela.esquema = pk.esquema
					and tabela.tabela = pk.tabela
					and tabela.campo = pk.campo_pk
				)
				left join 
				(SELECT
					n.nspname AS esquema,
					cl.relname AS tabela,
					a.attname AS campo,
					ct.conname AS constraint,
					nf.nspname AS esquema_fk,
					clf.relname AS tabela_fk,
					af.attname AS campo_fk
					--pg_get_constraintdef(ct.oid) AS criar_sql
				FROM 
					pg_catalog.pg_attribute a
					JOIN pg_catalog.pg_class cl ON (a.attrelid = cl.oid AND cl.relkind= 'r')
					JOIN pg_catalog.pg_namespace n ON (n.oid = cl.relnamespace)
					JOIN pg_catalog.pg_constraint ct ON (a.attrelid = ct.conrelid AND ct.confrelid != 0 AND ct.conkey[1] = a.attnum) 
					JOIN pg_catalog.pg_class clf ON (ct.confrelid = clf.oid AND clf.relkind = 'r')
					JOIN pg_catalog.pg_namespace nf ON (nf.oid = clf.relnamespace)
					JOIN pg_catalog.pg_attribute af ON (af.attrelid = ct.confrelid AND af.attnum = ct.confkey[1])
				) as fk
				on (
					tabela.esquema = fk.esquema
					and tabela.tabela = fk.tabela
					and tabela.campo = fk.campo
				)
				left join
                (
                    SELECT
                        ic.relname AS index_name,
                        bc.relname AS tab_name,
                        ta.attname AS column_name,
                        i.indisunique AS unique_key,
                        i.indisprimary AS primary_key
                    FROM
                        pg_class bc,
                        pg_class ic,
                        pg_index i,
                        pg_attribute ta,
                        pg_attribute ia
                    WHERE
                        bc.oid = i.indrelid
                        AND ic.oid = i.indexrelid
                        AND ia.attrelid = i.indexrelid
                        AND ta.attrelid = bc.oid
                        AND ta.attrelid = i.indrelid
                        AND ta.attnum = i.indkey[ia.attnum-1]
                    ORDER BY
                        index_name, tab_name, column_name
                ) as uk
                on
                (
                    tabela.tabela = uk.tab_name
                    and tabela.campo = uk.column_name
                )
                where
				tabela.esquema || '.' || tabela.tabela = '{$tabela}'
			order by
				tabela.esquema, 
				tabela.tabela,
				pk.campo_pk,
				fk.campo_fk	
sql
        );
    }

    public function debug(AbstractDado $dado)
    {
        ver($dado);
        ver($dado->mapear());
        ver($this->lerEstruturaBanco($dado->mapear()['tabela']));
    }
}
